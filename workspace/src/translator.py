import socket, threading, json, os, time

PORT = 7999
LOG_PATH = "/root/unlimited_grid_deck/active_matrix.log"

def translate_packet(meta):
    """Translates dense networking structures into plain, universal English."""
    try:
        file_name = meta.get('file', 'Unknown_Asset')
        bytes_vol = meta.get('size', 0)
        final_dest = meta.get('target', 0)
        seed_val = meta.get('seed', '0x4b6a')
        
        # Translate hexadecimal parameters dynamically
        try:
            ascii_text = bytes.fromhex(seed_val.replace('0x', '')).decode('utf-8', errors='ignore')
        except:
            ascii_text = 'N/A'
            
        decimal_val = int(seed_val, 16) if '0x' in seed_val else 19306
        
        log_entry = (
            f"\n--- [AUTOMATED TRANSLATION LAYER REGISTERED] ---\n"
            f"TIMESTAMP           : {time.strftime('%Y-%m-%d %H:%M:%S')}\n"
            f"PACKET ANALYSIS     : The system intercepted a file named '{file_name}'.\n"
            f"VOLUME METRIC       : The total file weight measures exactly {bytes_vol} bytes.\n"
            f"ROUTING DIRECTION   : This data packet is traveling through the mesh to settle at Node {final_dest}.\n"
            f"CIPHER UNPACKED     : Security Key {seed_val} translates directly to English letters '{ascii_text}' (Decimal ID: {decimal_val}).\n"
            f"STATUS              : Data decoded successfully. Pipeline flowing at maximum stability.\n"
            f"------------------------------------------------\n"
        )
        
        print(log_entry.replace('\n', '\n>>> '))
        with open(LOG_PATH, 'a') as f:
            f.write(log_entry)
    except Exception as e:
        print(f"Translation Failure: {e}")

def run_engine():
    s = socket.socket()
    s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    s.bind(('127.0.0.1', PORT))
    s.listen(10)
    print("\033[1;36m[ZIGGYOS CORE TRANSLATOR ACTIVE]\033[0m Awaiting active wire frames...\n>>> ", end="")
    
    while True:
        try:
            c, _ = s.accept()
            rl = b''
            while not rl.endswith(b'\n'): rl += c.recv(1)
            meta = json.loads(c.recv(int(rl.strip())).decode())
            translate_packet(meta)
            c.close()
        except: pass

threading.Thread(target=run_engine, daemon=True).start()
time.sleep(0.2)
import code; code.interact(banner="", local={'stats': lambda: print("Translator holding stable.")})
