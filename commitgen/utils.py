import os
import subprocess
import tempfile
from .signals import exit_with_footer

def fail(message: str, code: int = 1) -> None:
    exit_with_footer(code, f"Error: {message}")

def clean_first_line(text: str) -> str:
    if not text:
        return ""
    t = text.strip().split("```")[0].strip()
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
            edited = "".join(
                ln for ln in f.readlines() if not ln.lstrip().startswith("#")
            ).strip()
        return edited
    finally:
        try:
            os.remove(path)
        except OSError:
            pass

