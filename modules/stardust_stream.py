#!/usr/bin/env python3
# ==============================================================================
#  ZIGGYOS STARDUST STREAM ANIMATION ENGINE & LOCAL PACKET VECTOR
# ==============================================================================
import sys, socket, threading, time, random

GLYPHS = {
    'Z': ["██████", "   ██ ", "  ██  ", " ██   ", "██████"],
    'I': [" ████ ", "  ██  ", "  ██  ", "  ██  ", " ████ "],
    'G': [" ████ ", "██    ", "██ ███", "██  ██", " ████ "],
    'Y': ["██  ██", " ████ ", "  ██  ", "  ██  ", "  ██  "],
    'O': [" ████ ", "██  ██", "██  ██", "██  ██", " ████ "],
    'S': [" █████", "██    ", " ████ ", "    ██", "█████ "]
}
SPARKS = ['+', '*', '.', 'x', ' ']

def fire_background_packet(frame_id):
    """Fires a localized telemetry status frame directly at port 44777"""
    try:
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.settimeout(0.5)
        s.connect(("127.0.0.1", 44777))
        s.sendall(f"ZIGGY_FRAME_{frame_id}".encode())
        s.close()
    except:
        pass

def run_expanding_stream():
    frame = 0
    # Clear screen initially for clean display tracking
    sys.stdout.write("\033[2J\033[H")
    
    try:
        while True:
            frame += 1
            # Fire an asynchronous socket string payload to trigger the sniffer
            threading.Thread(target=fire_background_packet, args=(frame,), daemon=True).start()
            
            # Reset cursor positions cleanly to coordinates (0,0) to prevent vertical scrolling
            sys.stdout.write("\033[H")
            sys.stdout.write(f"\033[1;36m[+] ZIGGY-OS NETWORK STREAM | FRAME: {frame} | NODES: 75\033[0m\n\n")
            
            for row_idx in range(5):
                rendered_row = "  "
                for char in "ZIGGYOS":
                    if char in GLYPHS:
                        cell = GLYPHS[char][row_idx]
                        # Disperse sparkle particles into the structural block layers
                        for block in cell:
                            if block == ' ':
                                rendered_row += random.choice(SPARKS) if random.random() < 0.15 else ' '
                            else:
                                rendered_row += "\033[38;5;118m" + block + "\033[0m"
                    rendered_row += " "
                sys.stdout.write(f"{rendered_row}\n")
                
            sys.stdout.write(f"\n\033[94m[*] Processing swarm metrics on spectrum beacon 144.777 MHz...\033[0m\n")
            sys.stdout.flush()
            time.sleep(0.2)
    except KeyboardInterrupt:
        print("\n\033[93m[-] Connection stream closed cleanly.\033[0m")

if __name__ == "__main__":
    run_expanding_stream()
