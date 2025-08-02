import os
import subprocess
import sys
import signal # Import the signal module for Ctrl+C handling
from git import Repo, InvalidGitRepositoryError, GitCommandError
import google.generativeai as genai

# --- Configuration ---
# Replace with your actual Google Gemini API Key
API_KEY = "Add_Your_API_Key"
genai.configure(api_key=API_KEY)

MODEL_NAME = "gemini-2.5-flash" #fast
#MODEL_NAME = "gemini-2.5-pro"

# --- ASCII Art Banners ---
def print_header_banner():
    print("\n" + "="*60)
    print("        AI-POWERED GIT COMMIT GENERATOR")
    print("="*60 + "\n")

def print_footer_banner():
    print("\n" + "="*60)
    print("             Commit Process Finished")
    print("="*60 + "\n")

# --- Signal Handler for Ctrl+C ---
def signal_handler(sig, frame):
    print("\n\nCtrl+C detected. Exiting commit process gracefully.")
    sys.exit(0)

# Register the signal handler
signal.signal(signal.SIGINT, signal_handler)

# --- Helper Functions ---

def get_staged_diff():
    """Gets the diff of staged changes."""
    try:
        repo = Repo(os.getcwd())
        diff = repo.git.diff('--cached')
        if not diff:
            print("No staged changes found. Please stage your changes using 'git add .'")
            sys.exit(1)
        return diff
    except InvalidGitRepositoryError:
        print("Error: Not a Git repository.")
        sys.exit(1)
    except GitCommandError as e:
        print(f"Error executing Git command: {e}")
        sys.exit(1)

def generate_commit_message(diff_content):
    """Generates a commit message using the AI."""
    print("\n--- AI Message Generation ---")
    print("Generating commit message with AI... Please wait.")
    print("-----------------------------\n")
    try:
        model = genai.GenerativeModel(MODEL_NAME)
        # IMPORTANT: Keeping the original prompt text as requested
        prompt = f"""
		You are an AI that generates concise, conventional Git commit messages.

		Input: A Git diff.

		Task:
		- Analyze the diff and generate a single-line commit message.
		- Use the Conventional Commits format (e.g., feat:, fix:, docs:, refactor:, chore:).
		- Summarize the core change clearly and accurately.
		- Output only the commit message -- no explanations, comments, or code.

		Git Diff:
		```diff
		{diff_content}
		
		Commit Message:
        """
        response = model.generate_content(prompt)

        # Accessing text content, handling potential issues
        if response.parts:
            # Check if response.parts[0] has a text attribute
            if hasattr(response.parts[0], 'text'):
                return response.parts[0].text.strip()
            else:
                print("Warning: AI response part has no text attribute. Raw response:", response.parts[0])
                return "feat: AI generated message (no text content)"
        elif response.candidates and response.candidates[0].content.parts:
            return response.candidates[0].content.parts[0].text.strip()
        else:
            print("Error: AI did not return a valid message.")
            print(f"Full API response: {response}") # For debugging
            return "feat: AI generated message (error)"

    except Exception as e:
        print(f"Error calling AI model: {e}")
        print("Please check your API key and network connection.")
        return "feat: AI generation failed"

# --- Main Logic ---

if __name__ == "__main__":
    print_header_banner()
    diff = get_staged_diff()

    if not diff: # get_staged_diff already handles exit, but keeping this for clarity
        sys.exit(0)

    while True:
        commit_message = generate_commit_message(diff)

        print(f"\n--- Proposed Commit Message ---\n")
        print(f"{commit_message}\n")
        print(f"---------------------------------\n")

        # Improved prompt for user choice
        user_choice = input("Confirm (y), Regenerate (n), Edit (e), or Exit (x)? ").lower().strip()

        if user_choice == 'y':
            try:
                subprocess.run(['git', 'commit', '-m', commit_message], check=True)
                print("\nCommit successful!")
            except subprocess.CalledProcessError as e:
                print(f"Git commit failed: {e}")
                sys.exit(1)
            break # Exit loop after successful commit
        elif user_choice == 'n':
            print("\n---------------------------------")
            print("Regenerating a new commit message...")
            print("---------------------------------\n")
            # Loop continues to generate a new message
        elif user_choice == 'e':
            # Allow editing the message
            print("\n---------------------------------")
            print("Opening default editor to modify commit message...")
            print("---------------------------------\n")
            temp_file_path = "COMMIT_EDITMSG_AI"
            with open(temp_file_path, "w") as f:
                f.write(commit_message)

            try:
                # Use git commit -t FILE to open the default editor with content from FILE
                subprocess.run(['git', 'commit', '-t', temp_file_path], check=True)
                print("\nCommit successful after edit!")
            finally:
                # Clean up the temporary file
                if os.path.exists(temp_file_path):
                    os.remove(temp_file_path)
            break # Exit loop after successful commit
        elif user_choice == 'x':
            print("\nCommit process cancelled.")
            sys.exit(0)
        else:
            print("\nInvalid choice. Please enter 'y', 'n', 'e', or 'x'.")

    print_footer_banner()
