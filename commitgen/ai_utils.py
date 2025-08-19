"""
AI utilities for generating commit messages and descriptions using Google Gemini API.
"""

import os
import google.generativeai as genai

from commitgen.config import MODEL_NAME

# Configure API key
API_KEY = os.getenv("GEMINI_API_KEY")
if not API_KEY:
    raise RuntimeError("GEMINI_API_KEY not set. Add it to your environment or shell config.")

genai.configure(api_key=API_KEY)

def generate_commit_message(diff_content: str, staged_files: list) -> str:
    """
    Generate a single-line Conventional Commit message from the diff.
    """
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
    Generate a professional bullet-point commit description.
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

