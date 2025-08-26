import sys
import signal
from typing import Optional
from .banners import print_footer_banner

def exit_with_footer(code: int = 0, message: Optional[str] = None) -> None:
    if message:
        print(message)
    print_footer_banner()
    sys.exit(code)

def signal_handler(sig, frame) -> None:
    print("\n\nCtrl+C detected. Exiting gracefully.")
    exit_with_footer(0)

signal.signal(signal.SIGINT, signal_handler)

