import sys
import socket
import threading
import time
import random

# Core Interface Layout Constants
GLYPHS = {
    'Z': ["####", "  #/ ", " #/  ", "####"],
    'I': [" ###", "  # ", "  # ", " ###"],
    'G': [" ## ", "#   ", "# ##", " ## "],
    'Y': ["#  #", " ## ", "  # ", "  # "]
}
SPARKS = ['+', '*', '.', ' ', ' ']
TEAL = "\033[38;5;51m"
RESET = "\033[0m"

# Target Socket Ports for the 3 Geographic Zones
ZONE_PORTS = [44701, 44702, 44703]

# Thread-Safe Memory Queue for Logging
METRIC_QUEUE = []
queue_lock = threading.Lock()

def monitor_socket(port):
    """Background listener capturing data entries from active local zone networks."""
    while True:
        try:
            s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            s.settimeout(1.5)
            s.connect(("127.0.0.1", port))
            msg = s.recv(1024).decode('utf-8').strip()
            s.close()
            if msg:
                with queue_lock:
                    METRIC_QUEUE.append(msg)
                    if len(METRIC_QUEUE) > 8:
                        METRIC_QUEUE.pop(0)
        except Exception:
            pass
        time.sleep(0.2)

def run_cockpit():
    # Spin up background socket collectors for all 3 zones
    for port in ZONE_PORTS:
        t = threading.Thread(target=monitor_socket, args=(port,), daemon=True)
        t.start()
        
    frame = 0
    try:
        while True:
            frame += 1
            # Clear console space in-place
            sys.stdout.write("\033[H\033[J")
            sys.stdout.write(f"{TEAL}⚡ ZIGGY-OS COCKPIT | FRAME: {frame} | MULTI-ZONE STREAMING ACTIVE ⚡{RESET}\n\n")
            sys.stdout.write(f" 📍 \033[38;5;15mSPATIAL COORDINATES\033[0m : -36.790 S, 174.750 E (North Shore, AKL)\n")
            sys.stdout.write(f" 🛰️  \033[38;5;15mGLOBAL SWARM TOPO  \033[0m : 75 Nodes Active (NZ Spectrum Locked)\n")
            
            # 1. Render the Electric Teal Stardust Block Text
            for row_idx in range(4):
                rendered_row = ""
                for char in "ZIGGY":
                    if char in GLYPHS:
                        for cell in GLYPHS[char][row_idx]:
                            if cell == '#':
                                rendered_row += f"{TEAL}■{RESET}"
                            else:
                                rendered_row += random.choice(SPARKS) if random.random() < 0.15 else " "
                sys.stdout.write(f"  {rendered_row}\n")
                
            sys.stdout.write(f"\n{TEAL}" + "="*70 + f"{RESET}\n")
            sys.stdout.write(f"📡 LIVE P2P SUB-NODE TRANSACTION STREAM:\n")
            sys.stdout.write(f"{TEAL}" + "="*70 + f"{RESET}\n")
            
            # 2. Render the live streaming network metrics below the text
            with queue_lock:
                if not METRIC_QUEUE:
                    sys.stdout.write("  [*] Connecting to node loops on tracking spectrum...\n")
                else:
                    for metric in reversed(METRIC_QUEUE):
                        sys.stdout.write(f"  ⚡ {metric}\n")
                        
            sys.stdout.write(f"\n{TEAL}Press Ctrl+C to exit visualization.{RESET}\n")
            sys.stdout.flush()
            time.sleep(0.25)
    except KeyboardInterrupt:
        print("\n[+] Cockpit loop suspended. Returning to core shell prompt.")

if __name__ == "__main__":
    run_cockpit()
