from .signals import exit_with_footer

def ask(prompt: str, allowed: str) -> str:
    while True:
        try:
            ans = input(prompt).strip().lower()
        except EOFError:
            exit_with_footer(0, "\nInput closed. Exiting.")
        if ans and ans[0] in allowed:
            return ans[0]
        print(f"Invalid choice. Allowed: {', '.join(allowed)}.")

