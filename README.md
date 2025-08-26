# CommitGen

**CommitGen** is an AI-powered Git commit message generator that follows the **Conventional Commits** specification. It uses Google's Gemini model to suggest concise, professional commit messages and descriptions.

---

## ✨ Features
- Generates **Conventional Commit** titles (feat, fix, chore, etc.).
- Provides detailed bullet-point descriptions of changes.
- Interactive flow with options to **accept, regenerate, edit, or skip**.
- Integrates seamlessly with your Git workflow.

---

## ⚙️ Requirements
- Python **3.9+**
- [GitPython](https://pypi.org/project/GitPython/)
- [google-generativeai](https://pypi.org/project/google-generativeai/)
- A valid **Gemini API key** from Google AI Studio.

---

## 🚀 Installation
Clone the repository and install with pip:

```bash
git clone https://github.com/N0ViP/commitgen-project.git
cd commitgen-project
pip install -e .
```

This will install `commitgen` as a CLI command.

---

## 🔑 Setting up your API Key
CommitGen requires a Gemini API key. You can obtain one from [Google AI Studio](https://ai.google.dev/).

Export it as an environment variable:

```bash
export GEMINI_API_KEY="your_api_key_here"
```

Optionally, you can set the model:

```bash
export COMMITGEN_MODEL="gemini-2.5-flash"   # for faster responses
```

👉 If you notice **no response or empty output from the AI**, try switching the model to:
- `gemini-2.5-flash`
- `gemini-1.5-flash`
- `gemini-1.5-pro`

For permanent setup, add the exports to your shell profile (`~/.bashrc`, `~/.zshrc`, etc.).

---

## 📝 Usage
1. Stage your changes as usual:
   ```bash
   git add .
   ```
2. Run CommitGen:
   ```bash
   commitgen
   ```
3. Follow the interactive prompts:
   - Accept / regenerate / edit the suggested commit title.
   - Optionally provide or skip a description.
   - Confirm and commit.

---

## ⚠️ Potential Issues
- **No staged changes** → CommitGen will exit with an error until you run `git add`.
- **Missing API key** → Ensure `GEMINI_API_KEY` is exported in your environment.
- **Model errors or empty output** → Try changing the model as explained above.
- **Windows editor issues** → By default, CommitGen uses `notepad` on Windows and `nano` on Unix. Set `$EDITOR` to override.

---

## ❌ Uninstalling
To remove CommitGen:

```bash
pip uninstall commitgen
```

And if you cloned the repository, you can safely delete the project folder:

```bash
rm -rf commitgen
```

Remove the environment variables from your shell profile if you no longer need them.

---

## 🙋 FAQ
**Q: Can I still write my own commit messages?**  
Yes! CommitGen only helps when you run it. You can always use `git commit` directly.

**Q: Does it support multiple files and big diffs?**  
Yes, but generation time depends on the size of the diff and the chosen Gemini model.

**Q: Is my code sent to Google?**  
Yes, staged diffs are sent to the Gemini API to generate commit messages. Avoid staging sensitive information if this is a concern.

