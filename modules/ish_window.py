import os
import sys
import subprocess

TEAL = "\033[38;5;51m"
GREY = "\033[38;5;242m"
WHITE = "\033[38;5;15m"
RESET = "\033[0m"

def launch_nested_window():
    # Render the sub-window panel frame layout
    sys.stdout.write("\033[H\033[J")
    print(f"{TEAL}╭────────────────────────────────────────────────────────────────────╮{RESET}")
    print(f"{TEAL}│ 📟 ZIGGY-OS : NESTED ALPINE iSH CONTAINER CONSOLE WINDOW (v1.0)   │{RESET}")
    print(f"{TEAL}├────────────────────────────────────────────────────────────────────┤{RESET}")
    print(f"{TEAL}│{RESET}  System privilege  : Non-Root User Mode (Ring 3 Privilege Separation) {TEAL}│{RESET}")
    print(f"{TEAL}│{RESET}  Active Fork Track : TRACK_BETA (Inline Virtual Execution Pipeline)  {TEAL}│{RESET}")
    print(f"{TEAL}│{RESET}  Escape Command    : Type 'exit_window' to close the nested shell    {TEAL}│{RESET}")
    print(f"{TEAL}╰────────────────────────────────────────────────────────────────────╯{RESET}")
    print(f"{GREY}--- [NESTED ALPINE INTERMEDIATE LOOP ENGAGED] ---{RESET}\n")

    while True:
        try:
            # Custom nested prompt layout to differentiate from the base system shell
            cmd = input(f"⚡ {TEAL}[ziggy-os-window]:~#{RESET} ").strip()
            
            if not cmd:
                continue
            if cmd == "exit_window" or cmd == "exit":
                print(f"\n{GREY}[*] Detaching virtual shell window container...{RESET}")
                break
                
            # Intercept custom system commands or route standard instructions to the iSH kernel
            if cmd == "monitor":
                cmd = "python3 /root/teal_cockpit.py"
            elif cmd == "tag":
                cmd = "python3 /root/tag_generator.py"

            # Execute the command inside the native Alpine ash environment and pipe output back
            result = subprocess.run(cmd, shell=True, text=True)
            
        except KeyboardInterrupt:
            print(f"\n{GREY}[*] Use 'exit_window' to cleanly terminate the session context.{RESET}")
        except Exception as e:
            print(f"  [!] Container execution fault: {e}")

if __name__ == "__main__":
    launch_nested_window()
