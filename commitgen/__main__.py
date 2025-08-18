"""
Entry point for running commitgen as a standalone module.
This file allows `python -m commitgen` to work.
"""
from commitgen.cli import main
import sys
from commitgen.exceptions import GitRepoError, NoStagedChangesError

if __name__ == "__main__":
    try:
        sys.exit(main())
    except NoStagedChangesError:
        print("No staged changes found. Please stage your files using 'git add' before running commitgen.")
        sys.exit(0)
    except GitRepoError as e:
        print(f"Error: {e}")
        sys.exit(1)
    except KeyboardInterrupt:
        print("\nCtrl+C detected. Exiting commit process.")
        sys.exit(0)

