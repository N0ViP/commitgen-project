"""
AI utilities for generating commit messages and descriptions using Google Gemini API.
"""
import os
import google.generativeai as genai

# Configure API key
API_KEY = os.getenv("GEMINI_API_KEY")
if not API_KEY:
    raise RuntimeError("GEMINI_API_KEY not set")

genai.configure(api_key=API_KEY)
MODEL_NAME = "gemini-2.5-pro"

def generate_commit_message(diff_content: str, staged_files: list) -> str:
    """Generate a single-line conventional commit message from diff."""
    model = genai.GenerativeModel(MODEL_NAME)
    files_str = ", ".join(staged_files) if staged_files else "multiple files"

    prompt = f"""
You are an expert assistant that writes precise Git commit messages following the Conventional Commits specification.

Modified files: {files_str}
Diff:
{diff_content}

Generate only a single-line commit message.
"""
    response = model.generate_content(prompt)
    if response.parts and hasattr(response.parts[0], "text"):
        return response.parts[0].text.strip()
    return "feat: AI generated commit"

def improve_description(diff_content: str, raw_description: str, staged_files: list) -> str:
    """
    Generate a professional bullet-point description from diff and raw description.
    """
    model = genai.GenerativeModel(MODEL_NAME)
    files_str = ", ".join(staged_files) if staged_files else "multiple files"

    prompt = f"""
Generate a concise, bullet-point commit description from:
Diff:
{diff_content}
Files: {files_str}
Raw description:
{raw_description}
"""
    response = model.generate_content(prompt)
    if response.parts and hasattr(response.parts[0], "text"):
        return response.parts[0].text.strip()
    return raw_description

