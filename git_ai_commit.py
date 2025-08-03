#! /bin/python3

import os
import subprocess
import sys
import signal
from git import Repo, InvalidGitRepositoryError, GitCommandError  # type: ignore
import google.generativeai as genai  # type: ignore
import tempfile

# --- Configuration ---
API_KEY = os.getenv("GEMINI_API_KEY")
if not API_KEY:
    print("Error: Gemini API key not found. Please set GEMINI_API_KEY environment variable.")
    sys.exit(1)

genai.configure(api_key=API_KEY)

MODEL_NAME = "gemini-2.5-pro"    # for high-quality commits (slower)
# MODEL_NAME = "gemini-2.5-flash"    # for fast commits (less detailed)


# --- ASCII Art Banners ---
def print_header_banner():
    print("\n" + "=" * 60)
    print("        AI-POWERED GIT COMMIT GENERATOR")
    print("=" * 60 + "\n")


def print_footer_banner():
    print("\n" + "=" * 60)
    print("             Commit Process Finished")
    print("=" * 60 + "\n")


# --- Signal Handler for Ctrl+C ---
def signal_handler(sig, frame):
    print("\n\nCtrl+C detected. Exiting commit process gracefully.")
    sys.exit(0)


signal.signal(signal.SIGINT, signal_handler)


# --- Helper Functions ---
def get_staged_diff():
    try:
        repo = Repo(os.getcwd())
        diff = repo.git.diff('--cached')
        if not diff:
            print("No staged changes found. Please stage your changes using 'git add .'")
            sys.exit(1)
        return diff
    except InvalidGitRepositoryError:
        print("Error: Not a Git repository.")
        sys.exit(1)
    except GitCommandError as e:
        print(f"Error executing Git command: {e}")
        sys.exit(1)


def get_staged_files():
    try:
        repo = Repo(os.getcwd())
        files = repo.git.diff('--cached', '--name-only').splitlines()
        return files
    except Exception:
        return []


def generate_commit_message(diff_content, staged_files):
    print("\n--- AI Message Generation ---")
    print("Generating commit message with AI... Please wait.")
    print("-----------------------------\n")
    try:
        model = genai.GenerativeModel(MODEL_NAME)
        files_str = ", ".join(staged_files) if staged_files else "multiple files"
        prompt = f"""
You are an expert assistant that writes precise Git commit messages following the **Conventional Commits** specification.

Your task is to generate a **single-line commit message** in the following format:

<type>(<scope>): <imperative summary>

Guidelines:
- Choose one type from: feat, fix, refactor, chore, docs, style, test, build, ci, perf
- Derive the scope from the most relevant module, feature, or filename—use lowercase (e.g., parser, auth, api). If no clear scope exists, use "core" or "multiple".
- The summary must:
  - Be an imperative verb phrase (e.g., "add support for...").
  - Stay under 100 characters.
  - Clearly state what was changed and why or how (if possible).
- Use present tense verbs only. Avoid gerunds (e.g., "adding", "updating").
- Do **not** include:
  - Any punctuation beyond the message line
  - Extra commentary, markdown, or bullet points
  - Pronouns or subjective language

Inputs:
- Modified files: {files_str}
- Diff:
{diff_content}

Generate only the commit message (no additional output):
"""

        response = model.generate_content(prompt)
        if response.parts and hasattr(response.parts[0], 'text'):
            return response.parts[0].text.strip()
        elif response.candidates and response.candidates[0].content.parts:
            return response.candidates[0].content.parts[0].text.strip()
        else:
            return "feat: AI generated message (invalid response)"
    except Exception as e:
        print(f"Error calling AI model: {e}")
        return "feat: AI generation failed"


def improve_description(diff_content, raw_description, staged_files):
    try:
        model = genai.GenerativeModel(MODEL_NAME)
        files_str = ", ".join(staged_files) if staged_files else "multiple files"
        description_prompt = f"""
You are an expert assistant specialized in crafting clear, detailed, and professional Git commit descriptions from given inputs.

Inputs:
- A staged diff showing code changes.
- A raw user-written commit explanation (may be incomplete or missing).
- A list of modified files: {files_str}

Your task:
- Generate a concise, well-structured commit description in bullet points.
- Each bullet must start with a dash and describe a distinct, meaningful change.
- Provide exactly one bullet point per modified file, describing the change specific to that file.
- Use precise technical language; reference relevant functions, classes, modules, or components wherever possible.
- Focus on the impact and purpose of each change, emphasizing why the modification was necessary.
- Avoid vague, generic, or repetitive statements.
- Maintain a professional tone, keeping the description succinct yet informative.
- Do not include any formatting other than simple dashes (no markdown, numbering, or extra symbols).
- If the raw commit explanation is missing or insufficient, infer the most plausible changes from the filenames and diff content.

Below are the inputs to analyze:

Diff:
{diff_content}

Raw commit explanation:
\"\"\"
{raw_description}
\"\"\"

Generate the commit description below:
"""

        response = model.generate_content(description_prompt)
        if response.parts and hasattr(response.parts[0], 'text'):
            return response.parts[0].text.strip()
        return raw_description
    except Exception as e:
        print(f"AI enhancement failed: {e}")
        return raw_description


def edit_text_with_editor(initial_text):
    EDITOR = os.getenv('EDITOR', 'nano' if os.name != 'nt' else 'notepad')
    comment_hint = "# Enter your commit description below. Lines starting with # will be ignored.\n\n"
    initial_content = comment_hint + initial_text

    with tempfile.NamedTemporaryFile(suffix=".tmp", mode='w+', delete=False) as tf:
        tf.write(initial_content)
        tf.flush()
        temp_path = tf.name

    try:
        subprocess.run([EDITOR, temp_path], check=True)
        with open(temp_path, 'r') as f:
            edited_text = ''.join(
                line for line in f.readlines() if not line.strip().startswith('#')
            ).strip()
    finally:
        try:
            os.remove(temp_path)
        except OSError:
            pass
    return edited_text


def format_commit_message(title, description):
    commit_message = title
    if description:
        commit_message += f"\n\n{description}"
    return commit_message


# --- Main Logic ---
if __name__ == "__main__":
    print_header_banner()
    diff = get_staged_diff()
    staged_files = get_staged_files()

    commit_message = ""

    while True:
        if not commit_message:
            commit_message = generate_commit_message(diff, staged_files)

        print(f"\n--- Proposed Commit Title ---\n")
        print(f"{commit_message}\n")
        print(f"---------------------------------\n")

        user_choice = input("Confirm (y), Regenerate (n), Edit (e), or Quit (q)? ").lower().strip()

        if user_choice == 'y':
            while True:
                add_desc = input("Do you want to add a description or `a` for auto-generate? (y/n/a): ").lower().strip()
                if add_desc == 'n':
                    description = ""
                    break
                elif add_desc == 'y':
                    print("\nOpening editor to enter your own commit description...")
                    raw_description = edit_text_with_editor("")
                    if not raw_description.strip():
                        print("\nNo description entered. Skipping description.")
                        description = ""
                    else:
                        improved_description = improve_description(diff, raw_description, staged_files)
                        while True:
                            print("\n--- Improved Description ---\n")
                            print(improved_description + "\n")
                            desc_choice = input("Accept (y), Edit (e), Regenerate (n), or Skip (s)? ").lower().strip()
                            if desc_choice == 'y':
                                description = improved_description
                                break
                            elif desc_choice == 'e':
                                description = edit_text_with_editor(improved_description)
                                improved_description = description
                            elif desc_choice == 'n':
                                print("\nRegenerating improved description...\n")
                                improved_description = improve_description(diff, raw_description, staged_files)
                            elif desc_choice == 's':
                                description = ""
                                break
                            else:
                                print("Invalid choice. Please enter 'y', 'n', 'e', or 's'.")
                    break
                elif add_desc == 'a':
                    print("\nAuto-generating description based on file changes...")
                    raw_description = ""
                    improved_description = improve_description(diff, raw_description, staged_files)

                    while True:
                        print("\n--- Auto-Generated Description ---\n")
                        print(improved_description + "\n")
                        desc_choice = input("Accept (y), Edit (e), Regenerate (n), Skip (s), or Quit (q)? ").lower().strip()

                        if desc_choice == 'y':
                            description = improved_description
                            break
                        elif desc_choice == 'e':
                            description = edit_text_with_editor(improved_description)
                            improved_description = description
                        elif desc_choice == 'n':
                            print("\nRegenerating auto-generated description...\n")
                            improved_description = improve_description(diff, raw_description, staged_files)
                        elif desc_choice == 's':
                            description = ""
                            break
                        elif desc_choice == 'q':
                            print("\nCommit process cancelled during description editing.")
                            sys.exit(0)
                        else:
                            print("Invalid choice. Please enter 'y', 'n', 'e', 's', or 'q'.")
                    break
                else:
                    print("Invalid choice. Please enter 'y', 'n', or 'a'.")

            full_commit_message = format_commit_message(commit_message, description)

            try:
                subprocess.run(['git', 'commit', '-m', full_commit_message], check=True)
                print("\nCommit successful!")
            except subprocess.CalledProcessError as e:
                print(f"Git commit failed: {e}")
                sys.exit(1)
            break

        elif user_choice == 'n':
            print("\n---------------------------------")
            print("Regenerating a new commit message...")
            print("---------------------------------\n")
            commit_message = ""  # reset for regeneration

        elif user_choice == 'e':
            print("\n---------------------------------")
            print("Opening default editor to modify commit message...")
            print("---------------------------------\n")
            temp_file_path = "COMMIT_EDITMSG_AI"
            with open(temp_file_path, "w") as f:
                f.write(commit_message)

            try:
                EDITOR = os.getenv('EDITOR', 'nano' if os.name != 'nt' else 'notepad')
                subprocess.run([EDITOR, temp_file_path], check=True)
                with open(temp_file_path, 'r') as f:
                    commit_message = f.read().strip()
            finally:
                if os.path.exists(temp_file_path):
                    os.remove(temp_file_path)

        elif user_choice == 'q':
            print("\nCommit process cancelled.")
            sys.exit(0)

        else:
            print("\nInvalid choice. Please enter 'y', 'n', 'e', or 'q'.")

    print_footer_banner()

