import sys
import socket
import time
import random

GLYPHS = {
    'Z': ["#######", "    ## ", "   ##  ", "  ##   ", " ##    ", "##     ", "#######"],
    'I': ["#######", "  ###  ", "  ###  ", "  ###  ", "  ###  ", "  ###  ", "#######"],
    'G': [" ##### ", "##   ##", "##     ", "## ####", "##   ##", "##   ##", " ##### "],
    'Y': ["##   ##", "##   ##", " ## ## ", "  ###  ", "  ###  ", "  ###  ", "  ###  "],
    ' ': ["       ", "       ", "       ", "       ", "       ", "       ", "       "]
}
SPARKS = ['+', '*', '.', '✧', '✨', ' ', ' ']

def run_expanding_stream():
    print("[*] Establishing local handshake with loopback socket at 127.0.0.1:44777...")
    frame = 0
    
    try:
        # Create a connection window to the active telemetry pipeline
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.settimeout(2.0)
        s.connect(("127.0.0.1", 44777))
        print("[✓] Handshake locked. Extracting live telemetry blocks...")
        time.sleep(1)
    except Exception:
        print("[-] Active socket offline. Falling back to internal loop validation.")

    try:
        while True:
            frame += 1
            # Render a 7-line vertical space block
            sys.stdout.write("\033[H\033[J")
            sys.stdout.write(f"✧ ZIGGY-OS NETWORK STREAM ✧ FRAME: {frame} | NODES ACTIVE: 75 ✧\n\n")
            
            for row_idx in range(7):
                rendered_row = ""
                # Draw the target signature
                for char in "ZIGGY":
                    if char in GLYPHS:
                        for cell in GLYPHS[char][row_idx]:
                            if cell == '#':
                                rendered_row += "𝖅"
                            else:
                                rendered_row += random.choice(SPARKS) if random.random() < 0.20 else " "
                print(f"  {rendered_row}")
                
            sys.stdout.write(f"\n[+] Processing swarm metrics on spectrum beacon 144.777 MHz...\n")
            sys.stdout.flush()
            time.sleep(0.15)
    except KeyboardInterrupt:
        print("\n[+] Connection stream closed cleanly.")

if __name__ == "__main__":
    run_expanding_stream()
