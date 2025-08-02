# AI-Powered Git Commit Message Generator

This tool uses Google Gemini to generate concise and conventional Git commit messages based on your staged code changes.

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

> If not set, the script will use the fallback default key provided in the code, but this is not recommended.

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

3. You'll be presented with an AI-generated commit message.

4. Choose an action:

- `y` – Confirm and commit the message.
- `n` – Regenerate a new message using AI.
- `e` – Open the message in your editor before committing.
- `x` – Exit the script without committing.

---

## 🧼 Cleanup

The script temporarily creates a file `COMMIT_EDITMSG_AI` for edited commits. It is automatically deleted after the commit.

---

## ❓ Common Issues

- **No staged changes**: Make sure to run `git add .` before executing the script.

- **Invalid or missing API key**: Ensure you've set the `GEMINI_API_KEY` environment variable correctly.

- **Not a Git repository**: Make sure you're in the root of a valid Git repo.

---

## 📝 License

MIT License

---

## 🙌 Credits

Powered by:

- [GitPython](https://gitpython.readthedocs.io/)
- [Google Generative AI Python SDK](https://github.com/google/generative-ai-python)

