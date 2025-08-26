#! /bin/python3
# -*- coding: utf-8 -*-

import os
import sys
import signal
import subprocess
import tempfile
from typing import List, Optional

from git import Repo, InvalidGitRepositoryError, GitCommandError  # type: ignore
import google.generativeai as genai  # type: ignore


# ================================
# Configuration
# ================================
API_KEY = os.getenv("GEMINI_API_KEY")
MODEL_NAME = os.getenv("COMMITGEN_MODEL", "gemini-2.5-pro")  # set COMMITGEN_MODEL=gemini-2.5-flash for speed

BANNER_WIDTH = 60

# Single Repo instance (created later after header to show clean errors)
REPO: Optional[Repo] = None

# ================================
# Banners
# ================================
def print_header_banner() -> None:
    bar = "=" * BANNER_WIDTH
    print("\n" + bar)
    print("     AI-POWERED GIT COMMIT GENERATOR (Conventional Commits)")
    print(bar + "\n")


def print_footer_banner() -> None:
    bar = "=" * BANNER_WIDTH
    print("\n" + bar)
    print("                 Commit Process Finished")
    print(bar + "\n")


# ================================
# Graceful Exit & Signals
# ================================
def exit_with_footer(code: int = 0, message: Optional[str] = None) -> None:
    if message:
        print(message)
    print_footer_banner()
    sys.exit(code)


def signal_handler(sig, frame) -> None:
    # No traceback, just a sweet, clean exit
    print("\n\nCtrl+C detected. Exiting gracefully.")
    exit_with_footer(0)


signal.signal(signal.SIGINT, signal_handler)


# ================================
# Utilities
# ================================
def fail(message: str, code: int = 1) -> None:
    exit_with_footer(code, f"Error: {message}")


def require_api_key() -> None:
    if not API_KEY:
        fail("Gemini API key not found. Please set GEMINI_API_KEY environment variable.")


def init_repo() -> Repo:
    try:
        return Repo(os.getcwd())
    except InvalidGitRepositoryError:
        fail("Not a Git repository.")


def get_staged_diff(repo: Repo) -> str:
    try:
        diff = repo.git.diff("--cached")
        if not diff:
            fail("No staged changes found. Please stage your changes using 'git add .'")
        return diff
    except GitCommandError as e:
        fail(f"Error executing Git command: {e}")
        return ""  # unreachable


def get_staged_files(repo: Repo) -> List[str]:
    try:
        out = repo.git.diff("--cached", "--name-only")
        return [ln for ln in out.splitlines() if ln.strip()]
    except Exception:
        return []


def clean_first_line(text: str) -> str:
    """Return only the first non-empty line, stripped; remove code fences if any."""
    if not text:
        return ""
    t = text.strip()
    t = t.split("```")[0].strip()
    # keep only first non-empty line
    for line in t.splitlines():
        s = line.strip()
        if s:
            return s
    return ""


def open_in_editor(initial: str, hint: str = "") -> str:
    editor = os.getenv("EDITOR", "nano" if os.name != "nt" else "notepad")
    content = (hint + initial).rstrip() + ("\n" if not initial.endswith("\n") else "")
    with tempfile.NamedTemporaryFile(suffix=".tmp", mode="w+", delete=False) as tf:
        tf.write(content)
        tf.flush()
        path = tf.name
    try:
        subprocess.run([editor, path], check=True)
        with open(path, "r") as f:
            # Drop comment/hint lines
            edited = "".join(
                ln for ln in f.readlines() if not ln.lstrip().startswith("#")
            ).strip()
        return edited
    finally:
        try:
            os.remove(path)
        except OSError:
            pass


# ================================
# AI Helpers
# ================================
def ai_generate(prompt: str) -> str:
    try:
        model = genai.GenerativeModel(MODEL_NAME)
        resp = model.generate_content(prompt)
        # Try common response shapes; fall back to text attr
        text = ""
        if hasattr(resp, "text") and isinstance(resp.text, str) and resp.text.strip():
            text = resp.text
        elif getattr(resp, "parts", None) and hasattr(resp.parts[0], "text"):
            text = resp.parts[0].text
        elif getattr(resp, "candidates", None):
            c0 = resp.candidates[0]
            if getattr(c0, "content", None) and getattr(c0.content, "parts", None):
                text = c0.content.parts[0].text
        return text.strip()
    except Exception as e:
        return f""  # We will handle empty → error messages at call sites


def generate_commit_title(diff_content: str, staged_files: List[str]) -> str:
    files_str = ", ".join(staged_files) if staged_files else "multiple files"
    prompt = f"""
You are an expert assistant that writes precise Git commit messages following the **Conventional Commits** spec.

Task: Generate a single-line commit title in the form:
<type>(<scope>): <imperative summary>

Rules:
- type ∈ [feat, fix, refactor, chore, docs, style, test, build, ci, perf]
- scope: derive from the most relevant module/feature/filename; lowercase; if unclear use "core" or "multiple"
- summary: imperative, <= 100 chars, clearly says what changed and why/how if possible
- No trailing punctuation beyond the line; no extra commentary or markdown
- Present tense, no gerunds

Inputs:
- Modified files: {files_str}
- Diff:
{diff_content}

Generate only the commit title line without quotes or code fences:
"""
    out = ai_generate(prompt)
    title = clean_first_line(out)
    if not title:
        title = "chore(core): update changes"
    return title


def generate_description(diff_content: str, staged_files: List[str], user_notes: str) -> str:
    files_str = ", ".join(staged_files) if staged_files else "multiple files"
    prompt = f"""
You are an assistant generating a concise, professional commit description as plain text bullet points.

Inputs:
- Diff (staged):
{diff_content}

- Modified files: {files_str}

- User notes (may be empty):
\"\"\"{user_notes.strip() if user_notes else ""}\"\"\"

Requirements:
- Output only simple dash-leading bullets (e.g., "- Add X", "- Fix Y"), no numbering, no markdown extras.
- Each bullet describes a distinct, meaningful change; use precise technical language.
- Prefer referencing functions/classes/modules when evident from the diff.
- Emphasize purpose/impact when possible.
- Keep it succinct (5–12 bullets max). If changes are few, 2–5 bullets are fine.
- Do not include code fences or any text outside the bullets.

Generate only the bullet list:
"""
    out = ai_generate(prompt)
    if not out:
        return "- Describe changes (AI generation unavailable)\n- Provide purpose/impact for each modified area"
    # Keep as-is, but strip code fences if any snuck in
    cleaned = out.strip()
    if "```" in cleaned:
        cleaned = cleaned.split("```")[0].strip()
    return cleaned


# ================================
# Git Commit Execution
# ================================
def run_git_commit(title: str, description: str) -> None:
    try:
        if description.strip():
            # Use two -m flags to provide subject and body cleanly
            subprocess.run(["git", "commit", "-m", title, "-m", description], check=True)
        else:
            subprocess.run(["git", "commit", "-m", title], check=True)
        print("\nCommit successful!")
    except subprocess.CalledProcessError as e:
        fail(f"Git commit failed: {e}")


# ================================
# Interaction Helpers
# ================================
def ask(prompt: str, allowed: str) -> str:
    """Ask user, constrain to allowed letters (case-insensitive)."""
    while True:
        try:
            ans = input(prompt).strip().lower()
        except EOFError:
            # Treat as quit
            exit_with_footer(0, "\nInput closed. Exiting.")
        if ans and ans[0] in allowed:
            return ans[0]
        print(f"Invalid choice. Allowed: {', '.join(allowed)}.")


# ================================
# Main Flow (as requested)
# ================================
def main() -> None:
    print_header_banner()
    require_api_key()
    genai.configure(api_key=API_KEY)

    global REPO
    REPO = init_repo()

    diff = get_staged_diff(REPO)
    staged_files = get_staged_files(REPO)

    # -------- 1) Commit Title Phase --------
    print("\nGenerating initial commit suggestion, please wait...\n")
    title = generate_commit_title(diff, staged_files)

    while True:
        print("\n--- Proposed Commit Title ---\n")
        print(title + "\n")
        print("-" * BANNER_WIDTH + "\n")

        choice = ask("Accept (y), Regenerate (n), Edit (e), or Quit (q)? ", "yneq")
        if choice == "y":
            break
        elif choice == "n":
            title = generate_commit_title(diff, staged_files)
            continue
        elif choice == "e":
            edited = open_in_editor(
                title,
                hint="# Edit your commit title below. Lines starting with # will be ignored.\n\n"
            )
            if not edited:
                print("Empty title. Keeping previous title.")
            else:
                # Show edited title to allow fixing mistakes in next loop
                title = clean_first_line(edited)
            continue
        elif choice == "q":
            exit_with_footer(0, "\nCommit process cancelled by user.")

    # -------- 2) Description Phase --------
    want_desc = ask("\nDo you want a description? (y/n/q): ", "ynq")
    description = ""

    if want_desc == "q":
        exit_with_footer(0, "\nCommit process cancelled by user.")
    elif want_desc == "y":
        # Ask for optional notes/keywords
        print("\n(Optional) Add keywords/notes for the description.")
        print("Press Ctrl+D (Unix/Mac) or Ctrl+Z then Enter (Windows) to finish.")
        try:
                notes = sys.stdin.read().strip()
                print("\nNotes captured. Please wait...\n")
        except KeyboardInterrupt:
                # In case Ctrl+C happens while typing notes
                signal_handler(signal.SIGINT, None)  # graceful exit


        # Generate and confirm description
        while True:
            description = generate_description(diff, staged_files, notes)
            print("\n--- Proposed Description ---\n")
            print(description + "\n")
            print("-" * BANNER_WIDTH + "\n")
            dchoice = ask("Accept (y), Regenerate (n), Edit (e), or Skip (s)? ", "ynes")
            if dchoice == "y":
                break
            elif dchoice == "n":
                # regenerate using the same notes & diff
                continue
            elif dchoice == "e":
                edited = open_in_editor(
                    description,
                    hint="# Edit your commit description below. Lines starting with # will be ignored.\n\n"
                )
                if edited.strip():
                    description = edited.strip()
                    # loop again to confirm or re-edit
                    print("\n--- Edited Description ---\n")
                    print(description + "\n")
                    print("-" * BANNER_WIDTH + "\n")
                    d2 = ask("Accept (y), Edit again (e), or Skip (s)? ", "yes")
                    if d2 == "y":
                        break
                    elif d2 == "e":
                        continue
                    else:
                        description = ""
                        break
                else:
                    print("Empty description. Skipping.")
                    description = ""
                    break
            else:  # skip
                description = ""
                break

    # -------- 3) Perform Git Commit --------
    run_git_commit(title, description)

    # Always footer on finish
    print_footer_banner()


if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        # Just in case; normally handled by signal handler
        signal_handler(signal.SIGINT, None)

