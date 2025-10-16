import os

os.environ["GRPC_VERBOSITY"] = "NONE"
os.environ["GRPC_LOG_SEVERITY_LEVEL"] = "ERROR"


import sys
import signal
import google.generativeai as genai

from .config import API_KEY, REPO
from .config import BANNER_WIDTH
from .banners import print_header_banner, print_footer_banner
from .signals import exit_with_footer, signal_handler
from .utils import fail, open_in_editor, clean_first_line
from .git_helpers import init_repo, get_staged_diff, get_staged_files
from .ai_helpers import generate_commit_title, generate_description
from .commit import run_git_commit
from .interaction import ask

signal.signal(signal.SIGINT, signal_handler)


def main() -> None:
    print_header_banner()
    if not API_KEY:
        fail("Gemini API key not found. Please set GEMINI_API_KEY environment variable.")
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
            if edited:
                title = clean_first_line(edited)
            else:
                print("Empty title. Keeping previous title.")
            continue
        elif choice == "q":
            exit_with_footer(0, "\nCommit process cancelled by user.")

    # -------- 2) Description Phase --------
    want_desc = ask("\nDo you want a description? (y/n/q): ", "ynq")
    description = ""

    if want_desc == "q":
        exit_with_footer(0, "\nCommit process cancelled by user.")
    elif want_desc == "y":
        print("\n(Optional) Add keywords/notes for the description.")
        print("Press Ctrl+D (Unix/Mac) or Ctrl+Z then Enter (Windows) to finish.")
        try:
            notes = sys.stdin.read().strip()
            print("\nNotes captured. Please wait...\n")
        except KeyboardInterrupt:
            signal_handler(signal.SIGINT, None)

        while True:
            description = generate_description(diff, staged_files, notes, title)
            print("\n--- Proposed Description ---\n")
            print(description + "\n")
            print("-" * BANNER_WIDTH + "\n")
            dchoice = ask("Accept (y), Regenerate (n), Edit (e), or Skip (s)? ", "ynes")
            if dchoice == "y":
                break
            elif dchoice == "n":
                continue
            elif dchoice == "e":
                edited = open_in_editor(
                    description,
                    hint="# Edit your commit description below. Lines starting with # will be ignored.\n\n"
                )
                if edited.strip():
                    description = edited.strip()
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
            else:
                description = ""
                break

    # -------- 3) Perform Git Commit --------
    run_git_commit(title, description)

    print_footer_banner()


if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        signal_handler(signal.SIGINT, None)

