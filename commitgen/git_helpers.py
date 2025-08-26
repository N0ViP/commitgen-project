import os
from git import Repo, InvalidGitRepositoryError, GitCommandError
from .utils import fail

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

def get_staged_files(repo: Repo):
    try:
        out = repo.git.diff("--cached", "--name-only")
        return [ln for ln in out.splitlines() if ln.strip()]
    except Exception:
        return []

