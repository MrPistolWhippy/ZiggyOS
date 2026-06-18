import socket
import time

def start_p2p_beacon(port=9999):
    # Initialize a standard user-space UDP socket
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    # Enable broadcast capability to mimic open array flooding
    sock.setsockopt(socket.SOL_SOCKET, socket.SO_BROADCAST, 1)
    
    print(f"[+] Free-Standing P2P Array Beacon Online on Port {port}...")
    
    try:
        while True:
            # Simulate a simulated payload synchronization frame
            payload_frame = b"[SHARD_SYNC] LEDGER_BLOCK:144 | ADDR:NZ_NODE_01"
            
            # Broadcast to localhost / local network space
            sock.sendto(payload_frame, ('255.255.255.255', port))
            print("[->] Transmitted payload frame to global array network matrix.")
            time.sleep(5)
    except KeyboardInterrupt:
        print("\n[-] Beacon deactivated safely.")

start_p2p_beacon()
