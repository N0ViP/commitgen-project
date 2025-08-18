# CommitGen – AI-Powered Git Commit Generator

---

## Overview

CommitGen is a professional tool that generates **Conventional Commits-compliant messages** using **Google Gemini AI**. It analyzes your **staged Git changes** and can optionally create **multi-line bullet-point descriptions** for each commit.

Key features:

* Single-line AI-generated commit messages
* Optional detailed descriptions per modified file
* Interactive workflow: confirm, regenerate, edit, skip, quit
* Handles edge cases: empty staging area, invalid API key, not a Git repository

---

## Requirements

* **Python 3.8+**
* **Git**
* Python packages:

```bash
pip install gitpython google-generativeai
```

---

## API Key Setup

CommitGen requires a **Google Gemini API key**.

1. Create a key at [Google AI Studio](https://aistudio.google.com/app/apikey)
2. Set the environment variable:

```bash
export GEMINI_API_KEY="your_api_key_here"
```

3. Reload your shell or add it to your shell config (`.zshrc` or `.bashrc`) to make it permanent.

---

## Quick Start

### Run from source

```bash
cd /path/to/commitgen-project
python -m commitgen
```

### Run globally (optional)

Install via pip for global use:

```bash
pip install git+https://github.com/<your-username>/commitgen.git
```

Then you can run it from any Git repository:

```bash
git commitgen
```

---

## Usage

1. Stage your changes:

```bash
git add .
```

2. Run CommitGen:

```bash
git commitgen
```

3. Follow the prompts:

| Option | Action                             |
| ------ | ---------------------------------- |
| `y`    | Confirm and commit AI message      |
| `n`    | Regenerate commit message          |
| `e`    | Edit message manually              |
| `q`    | Quit without committing            |
| `a`    | Auto-generate detailed description |
| `r`    | Regenerate description             |
| `s`    | Skip description                   |
| `e`    | Edit description manually          |

### Example

```
--- Proposed Commit Title ---
feat: add user authentication module

Confirm (y), Regenerate (n), Edit (e), or Quit (q)?
y
Add description (y), auto-generate (a), or skip (n)?
a
[main abc1234] feat: add user authentication module
 3 files changed, 25 insertions(+), 2 deletions(-)
Commit successful!
```

---

## Optional Git Alias

Add a Git alias for convenience:

```ini
[alias]
    commitgen = "!python3 /path/to/commitgen-project/__main__.py"
```

Then run it as:

```bash
git commitgen
```

---

## Features & Safety

* Checks if inside a Git repository
* Verifies staged changes before committing
* Handles empty or missing descriptions gracefully
* Supports regeneration or manual editing of AI suggestions
* Deletes temporary files automatically
* Validates API key presence

---

## Project Structure

```
commitgen/
├── __init__.py       # Package initializer
├── __main__.py       # CLI entry point
├── cli.py            # Interactive workflow logic
├── config.py         # Banner & configuration
├── editor.py         # Editor integration for manual input
├── ai_utils.py       # AI commit message generation
├── git_utils.py      # Git helpers for diff and commit
└── exceptions.py     # Custom exceptions for robust handling
```

---

## Contributing

* Follow modular, readable Python conventions
* Add inline comments for clarity
* Open to issues and pull requests on GitHub

---

## License

MIT License © 2025

