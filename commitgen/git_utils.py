"""
Git utilities: get staged diff, list files, run commit.
"""
from git import Repo, InvalidGitRepositoryError, GitCommandError
import subprocess
from commitgen.exceptions import GitRepoError, NoStagedChangesError

def get_staged_diff() -> str:
    """Return the git staged diff, or raise NoStagedChangesError."""
    try:
        repo = Repo(".")
        diff = repo.git.diff("--cached")
        if not diff:
            raise NoStagedChangesError()
        return diff
    except InvalidGitRepositoryError:
        raise GitRepoError("Not a Git repository.")
    except GitCommandError as e:
        raise GitRepoError(str(e))

def get_staged_files() -> list:
    """Return a list of staged filenames."""
    try:
        repo = Repo(".")
        return repo.git.diff("--cached", "--name-only").splitlines()
    except Exception:
        return []

def run_git_commit(message: str):
    """Run git commit with provided message."""
    subprocess.run(["git", "commit", "-m", message], check=True)

