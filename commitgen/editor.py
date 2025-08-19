"""
Helper functions for text editing using system editor.
"""

import os
import subprocess
import tempfile

def edit_text_with_editor(initial_text: str) -> str:
    """
    Open a temporary file in user's preferred editor to edit text.

    Lines starting with '#' are ignored.

    Args:
        initial_text (str): The initial text to prefill in editor.

    Returns:
        str: Edited text.
    """
    EDITOR = os.getenv("EDITOR", "nano" if os.name != "nt" else "notepad")
    comment_hint = "# Enter text below. Lines starting with # will be ignored.\n\n"
    initial_content = comment_hint + initial_text

    with tempfile.NamedTemporaryFile(suffix=".tmp", mode="w+", delete=False) as tf:
        tf.write(initial_content)
        tf.flush()
        temp_path = tf.name

    try:
        subprocess.run([EDITOR, temp_path], check=True)
        with open(temp_path, "r") as f:
            edited_text = "".join(line for line in f if not line.startswith("#")).strip()
    finally:
        os.remove(temp_path)
    return edited_text

