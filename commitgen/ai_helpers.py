from .config import API_KEY, MODEL_NAME
from .utils import clean_first_line

MAX_DIFF_CHARS = 8000

def _truncate_diff(diff: str) -> str:
    if len(diff) <= MAX_DIFF_CHARS:
        return diff
    return diff[:MAX_DIFF_CHARS] + "\n\n[... diff truncated for brevity ...]"

_HAS_NEW_SDK = False
_HAS_OLD_SDK = False

try:
    from google import genai as new_genai
    _HAS_NEW_SDK = True
except Exception:
    new_genai = None

if not _HAS_NEW_SDK:
    try:
        import google.generativeai as old_genai
        _HAS_OLD_SDK = True
    except Exception:
        old_genai = None

def ai_generate(prompt: str) -> str:
    try:
        if not API_KEY:
            return ""
        if _HAS_NEW_SDK:
            client = new_genai.Client(api_key=API_KEY)
            resp = client.models.generate_content(
                model=MODEL_NAME,
                contents=prompt,
                config=new_genai.types.GenerateContentConfig(
                    automatic_function_calling=new_genai.types.AutomaticFunctionCallingConfig(
                        disable=True,
                    ),
                ),
            )
        elif _HAS_OLD_SDK:
            old_genai.configure(api_key=API_KEY)
            model = old_genai.GenerativeModel(MODEL_NAME)
            resp = model.generate_content(prompt)
        else:
            return ""
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
    except Exception as e:
        import sys
        print(f"\n[commitgen] AI error: {e}", file=sys.stderr)
        return ""

def generate_commit_title(diff_content: str, staged_files: list[str]) -> str:
    files_str = ", ".join(staged_files) if staged_files else "multiple files"
    diff_content = _truncate_diff(diff_content)
    prompt = f"""
You are a senior software engineer generating a **Conventional Commit title** based on code changes.

Guidelines:
- Output ONLY ONE line — no explanations, no extra text.
- Format: type(scope): summary
- Use one of these types: feat, fix, refactor, style, docs, test, chore, perf, ci, build, revert.
- The scope should be concise and relevant (e.g., a filename, folder, or feature).
- The summary should describe what changed, using imperative mood ("add", "update", "fix", "remove").
- Keep the entire title under 70 characters.
- Do NOT include punctuation at the end, emojis, code snippets, backticks, or quotes around the output.

Example output:
feat(auth): add JWT token refresh on session expiry

Context:
- Changed files: {files_str}
- Git diff:
{diff_content}

Return only the final commit title as plain text.
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
    diff_content = _truncate_diff(diff_content)
    prompt = f"""
You are a professional assistant writing a **Conventional Commit description** that complements this title:
"{commit_title}"

Guidelines:
- Expand on the title — explain what changed and why.
- Use bullet points (starting with "- ") for clarity and structure.
- Mention affected files or modules if relevant.
- Do NOT repeat the title verbatim; provide supporting detail instead.
- Keep a professional and concise tone.
- Do NOT include markdown headers, code blocks, backticks, or commit hashes.
- Do NOT wrap your output in markdown formatting of any kind.
- If user notes exist, use them to enrich the context.

Example output:
- Added session refresh handler to prevent token expiry during long sessions
- Updated auth middleware to validate token lifetime before each request
- Removed deprecated legacy auth fallback logic

Context:
- Changed files: {files_str}
- User notes: {user_notes if user_notes else "None"}
- Git diff:
{diff_content}

Return only the formatted bullet-point description as plain text.
"""
    out = ai_generate(prompt)
    if not out:
        return "- Describe changes (AI unavailable)\n- Provide purpose/impact"
    cleaned = out.strip()
    if "```" in cleaned:
        cleaned = cleaned.split("```")[0].strip()
    return cleaned


