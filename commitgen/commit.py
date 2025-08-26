import subprocess
from .utils import fail

def run_git_commit(title: str, description: str) -> None:
    try:
        if description.strip():
            subprocess.run(["git", "commit", "-m", title, "-m", description], check=True)
        else:
            subprocess.run(["git", "commit", "-m", title], check=True)
        print("\nCommit successful!")
    except subprocess.CalledProcessError as e:
        fail(f"Git commit failed: {e}")

