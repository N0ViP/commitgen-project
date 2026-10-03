import os
from typing import Optional
from git import Repo

API_KEY = os.getenv("GEMINI_API_KEY")
MODEL_NAME = os.getenv("COMMITGEN_MODEL", "gemini-3.8-flash")
BANNER_WIDTH = 60

# Single Repo instance
REPO: Optional[Repo] = None

