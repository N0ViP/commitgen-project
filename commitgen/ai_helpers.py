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
You are an expert assistant that writes precise Git commit messages...
...
"""
    out = ai_generate(prompt)
    title = clean_first_line(out)
    return title if title else "chore(core): update changes"

def generate_description(diff_content: str, staged_files: list[str], user_notes: str) -> str:
    files_str = ", ".join(staged_files) if staged_files else "multiple files"
    prompt = f"""
You are an assistant generating a concise, professional commit description...
...
"""
    out = ai_generate(prompt)
    if not out:
        return "- Describe changes (AI unavailable)\n- Provide purpose/impact"
    cleaned = out.strip()
    if "```" in cleaned:
        cleaned = cleaned.split("```")[0].strip()
    return cleaned

