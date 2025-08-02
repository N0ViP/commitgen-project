# AI-Powered Git Commit Message Generator

This tool uses *Google Gemini* to generate concise and conventional Git commit messages based on your staged code changes.

---

## 📦 Requirements

Make sure you have the following installed:

- **Python 3.8+**
- **Git**
- Python packages:
  - `gitpython`
  - `google-generativeai`

Install required packages:

```bash
pip install gitpython google-generativeai
```

---

## 🔐 API Key Setup

To use Gemini, you need an API key from Google.

1. Go to [Google AI Studio](https://aistudio.google.com/app/apikey) and create an API key.
2. Set it as an environment variable:

```bash
export GEMINI_API_KEY="your_api_key_here"
```

3. To make this permanent, add the line to your shell config file:

```bash
# For Zsh
echo 'export GEMINI_API_KEY="your_api_key_here"' >> ~/.zshrc

# Or for Bash
echo 'export GEMINI_API_KEY="your_api_key_here"' >> ~/.bashrc
```

4. Then reload your shell:

```bash
source ~/.zshrc   # or source ~/.bashrc
```

---

## ⚙️ How to Use

1. Stage your changes in a Git repository:

```bash
git add .
```

2. Run the script:

```bash
python commitgen.py
```

3. You’ll be presented with an AI-generated commit message.

4. Choose an action:

- `y` – Confirm and commit the message.
- `n` – Regenerate a new message using AI.
- `e` – Edit manually before committing.
- `x` – Exit the script without committing.

---

## 🪄 Optional Git Alias

To run the script more easily, add a Git alias:

1. Open your Git config:

```bash
git config --global --edit
```

2. Add the following under `[alias]`:

```ini
[alias]
    ai = "!python3 /home/yjaafar/Desktop/scripts/git_ai_commit.py"
```

Now you can use:

```bash
git ai
```

---

## 🧼 Cleanup

The script creates a temporary file `COMMIT_EDITMSG_AI` when editing. It deletes it automatically after committing.

---

## ❓ Common Issues

- **No staged changes**: Run `git add .` first.
- **Missing or invalid API key**: Make sure `GEMINI_API_KEY` is correctly set.
- **Not a Git repository**: Ensure you're inside a valid Git repo.

---

## 📝 License

MIT License

---

## 🙌 Credits

Powered by:

- [GitPython](https://gitpython.readthedocs.io/)
- [Google Generative AI Python SDK](https://github.com/google/generative-ai-python)
