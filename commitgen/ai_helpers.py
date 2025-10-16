import google.generativeai as genai
from .config import MODEL_NAME
from .utils import clean_first_line

def ai_generate(prompt: str) -> str:
    try:
        model = genai.GenerativeModel(MODEL_NAME)
        resp = model.generate_content(prompt)
        text = ""
        if hasattr(resp, "text") and isinstance(resp.text, str) and resp.text.strip():
            text = resp.text
        elif getattr(resp, "parts", None) and hasattr(resp.parts[0], "text"):
            text = resp.parts[0].text
        elif getattr(resp, "candidates", None):
            c0 = resp.candidates[0]
            if getattr(c0, "content", None) and getattr(c0.content, "parts", None):
                text = c0.content.parts[0].text
        return text.strip()
    except Exception:
        return ""

def generate_commit_title(diff_content: str, staged_files: list[str]) -> str:
    files_str = ", ".join(staged_files) if staged_files else "multiple files"
    prompt = f"""
You are a senior software engineer generating a **Conventional Commit title** based on code changes.

Guidelines:
- Output ONLY ONE line.
- Format: `<type>(<scope>): <summary>`
- Use one of these types: feat, fix, refactor, style, docs, test, chore, perf, ci, build, revert.
- The <scope> should be concise and relevant (e.g., a filename, folder, or feature).
- The <summary> should describe what changed, using imperative mood ("add", "update", "fix", "remove").
- Keep it under 70 characters.
- Do NOT include punctuation at the end, emojis, or code snippets.

Context:
- Changed files: {files_str}
- Git diff:
{diff_content}

Return only the final commit title.
"""
    out = ai_generate(prompt)
    title = clean_first_line(out)
    return title if title else "chore(core): update changes"

def generate_description(
    diff_content: str,
    staged_files: list[str],
    user_notes: str,
    commit_title: str
) -> str:
    files_str = ", ".join(staged_files) if staged_files else "multiple files"
    prompt = f"""
You are a professional assistant writing a **Conventional Commit description** that complements this title:
"{commit_title}"

Guidelines:
- The description should expand on the title — explain what changed and why.
- Use bullet points for clarity and structure.
- Mention affected files or modules if relevant.
- Avoid repeating the title verbatim; instead, provide supporting detail.
- Keep a professional and concise tone.
- Do NOT include markdown headers, code blocks, or commit hashes.
- If user notes exist, use them to enrich the context.

Example format:
- Added `feature_x.c` to handle input validation
- Updated `utils.c` for better memory management
- Improved error logging in `main.c`

Context:
- Changed files: {files_str}
- User notes: {user_notes if user_notes else "None"}
- Git diff:
{diff_content}

Return only the formatted bullet-point description.
"""
    out = ai_generate(prompt)
    if not out:
        return "- Describe changes (AI unavailable)\n- Provide purpose/impact"
    cleaned = out.strip()
    if "```" in cleaned:
        cleaned = cleaned.split("```")[0].strip()
    return cleaned


