from .config import BANNER_WIDTH

def print_header_banner() -> None:
    bar = "=" * BANNER_WIDTH
    print("\n" + bar)
    print("     AI-POWERED GIT COMMIT GENERATOR (Conventional Commits)")
    print(bar + "\n")

def print_footer_banner() -> None:
    bar = "=" * BANNER_WIDTH
    print("\n" + bar)
    print("                 Commit Process Finished")
    print(bar + "\n")

