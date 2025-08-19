"""
CLI logic for CommitGen.
"""

import sys
from commitgen.ai_utils import generate_commit_message, improve_description
from commitgen.editor import edit_text_with_editor
from commitgen.git_utils import get_staged_diff, get_staged_files, run_git_commit
from commitgen.config import HEADER_BANNER, FOOTER_BANNER
from commitgen.exceptions import GitRepoError, NoStagedChangesError

def description_workflow(diff: str, staged_files: list) -> str:
    """
    Interactive workflow to generate or edit commit description.
    """
    description = ""
    while True:
        choice = input("Add description (y), auto-generate (a), or skip (n)? ").lower().strip()
        if choice == "n":
            return ""
        elif choice == "y":
            raw = edit_text_with_editor("")
            description = improve_description(diff, raw, staged_files)
            return description
        elif choice == "a":
            raw = improve_description(diff, "", staged_files)
            # Remove first line if it looks like a commit title
            lines = raw.splitlines()
            if lines and lines[0].split(":")[0].lower() in [
                "feat", "fix", "build", "refactor", "chore", "docs", "test", "style"
            ]:
                description = "\n".join(lines[1:]).strip()
            else:
                description = raw
            return description
        else:
            print("Invalid choice. Enter y/n/a.")

def main():
    """Main CLI workflow for CommitGen."""
    print(HEADER_BANNER)
    try:
        diff = get_staged_diff()
        staged_files = get_staged_files()
        title = generate_commit_message(diff, staged_files)
        print("\n--- Proposed Commit Title ---")
        print(title + "\n")
        confirm = input("Confirm (y), Regenerate (n), Edit (e), or Quit (q)? ").lower().strip()
        if confirm == "q":
            print("Commit cancelled by user.")
            return 0
        elif confirm == "n":
            title = generate_commit_message(diff, staged_files)
        elif confirm == "e":
            title = edit_text_with_editor(title)

        description = description_workflow(diff, staged_files)

        commit_message = title
        if description:
            commit_message += "\n\n" + description

        run_git_commit(commit_message)
        print("\nCommit successful!")
    except NoStagedChangesError:
        print("No staged changes found. Please stage files using 'git add'.")
    except GitRepoError as e:
        print(f"Error: {e}")
    except KeyboardInterrupt:
        print("\nCtrl+C detected. Exiting commit process.")
    finally:
        print(FOOTER_BANNER)

