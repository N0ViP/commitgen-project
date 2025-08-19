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

3. To make it permanent, add the line to your shell config file:

```bash
# For Zsh
echo 'export GEMINI_API_KEY="your_api_key_here"' >> ~/.zshrc

# For Bash
echo 'export GEMINI_API_KEY="your_api_key_here"' >> ~/.bashrc
```

4. Reload your shell:

```bash
source ~/.zshrc   # or source ~/.bashrc
```

---

## Quick Start

### Run from source

```bash
cd /path/to/commitgen-project
python3 -m commitgen
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

### Example

```
============================================================
        AI-POWERED GIT COMMIT GENERATOR
============================================================

--- Proposed Commit Title ---
build: remove project metadata and build configuration

Confirm (y), Regenerate (n), Edit (e), or Quit (q)? y
Add description (y), auto-generate (a), or skip (n)? a

--- Proposed Commit Description ---

- Remove build, project, and dependency configuration from `pyproject.toml`.
- Delete the `git-commitgen` script alias.

Accept (y), Edit (e), Regenerate (n), Skip (s), Quit (q)? y
[main 053cf4a] build: remove project metadata and build configuration
 2 files changed, 3 insertions(+), 25 deletions(-)
 rewrite pyproject.toml (88%)

Commit successful!

============================================================
             Commit Process Finished
============================================================
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

## Removing CommitGen

To completely remove CommitGen from your system:

1. If installed globally via pip:

```bash
pip uninstall commitgen
```

2. If run from source, delete the project folder:

```bash
rm -rf /path/to/commitgen-project
```

This removes all scripts and files related to the tool.

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

Contributions are welcome! To contribute:

* Follow modular, readable Python conventions.
* Include clear inline comments for code clarity.
* Open issues or submit pull requests on GitHub to suggest improvements or fixes.

