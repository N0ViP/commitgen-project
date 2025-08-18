"""
Command-line interface and main workflow for commitgen.
Handles user interaction for generating commit messages and descriptions.
"""
import sys
from commitgen.config import print_header_banner, print_footer_banner
from commitgen.editor import edit_text_with_editor
from commitgen.ai_utils import generate_commit_message, improve_description
from commitgen.git_utils import get_staged_diff, get_staged_files, run_git_commit
from commitgen.exceptions import GitRepoError, NoStagedChangesError

def main():
    """Main CLI workflow."""
    print_header_banner()

    # Get git staged diff and files
    try:
        diff = get_staged_diff()
        staged_files = get_staged_files()
    except NoStagedChangesError:
        print("No staged changes found. Please stage your files using 'git add' before running commitgen.")
        sys.exit(0)
    except GitRepoError as e:
        print(f"Error: {e}")
        sys.exit(1)

    commit_message = ""

    while True:
        # Generate commit message if empty
        if not commit_message:
            commit_message = generate_commit_message(diff, staged_files)

        print(f"\n--- Proposed Commit Title ---\n{commit_message}\n")
        choice = input("Confirm (y), Regenerate (n), Edit (e), or Quit (q)? ").lower().strip()

        if choice == 'y':
            description = _description_workflow(diff, staged_files)
            full_message = commit_message
            if description:
                full_message += f"\n\n{description}"

            try:
                run_git_commit(full_message)
                print("\nCommit successful!")
            except Exception as e:
                print(f"Git commit failed: {e}")
                sys.exit(1)
            break
        elif choice == 'n':
            commit_message = ""  # regenerate
        elif choice == 'e':
            commit_message = edit_text_with_editor(commit_message)
        elif choice == 'q':
            print("Commit process cancelled.")
            sys.exit(0)
        else:
            print("Invalid choice. Enter y/n/e/q.")

    print_footer_banner()


def _description_workflow(diff, staged_files):
    """
    Handles commit description workflow.
    Options: add manually (y), auto-generate (a), skip (n)
    """
    while True:
        add_desc = input("Add description (y), auto-generate (a), or skip (n)? ").lower().strip()

        if add_desc == 'n':
            return ""
        elif add_desc == 'y':
            raw_description = edit_text_with_editor("")
            description = improve_description(diff, raw_description, staged_files) if raw_description else ""
            return _confirm_description(description, diff, staged_files)
        elif add_desc == 'a':
            description = improve_description(diff, "", staged_files)
            return _confirm_description(description, diff, staged_files)
        else:
            print("Invalid choice. Enter y/n/a.")


def _confirm_description(description, diff, staged_files):
    """
    Confirm or modify the commit description.
    Options: Accept (y), Edit (e), Regenerate (n), Skip (s), Quit (q)
    """
    while True:
        print("\n--- Proposed Commit Description ---\n")
        print(description + "\n" if description else "(No description generated)\n")

        choice = input("Accept (y), Edit (e), Regenerate (n), Skip (s), Quit (q)? ").lower().strip()
        if choice == 'y':
            return description
        elif choice == 'e':
            description = edit_text_with_editor(description)
        elif choice == 'n':
            description = improve_description(diff, "", staged_files)
        elif choice == 's':
            return ""
        elif choice == 'q':
            print("Commit process cancelled during description editing.")
            sys.exit(0)
        else:
            print("Invalid choice. Enter y/e/n/s/q.")

