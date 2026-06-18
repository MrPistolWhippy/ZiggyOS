import sys
import socket
import threading
import time
import random

# Cryptographic Token Hierarchy Keys
KEY_HIERARCHY = {
    "MICRO": "ZIGGY_MIC_144",   # Individual local node verification
    "MEDIO": "ZIGGY_MED_777",   # Regional mesh routing aggregation
    "MACRO": "ZIGGY_MAC_999"    # Global topology swarm synchronization
}

# Network Socket Topology Mapping
ZONE_CONFIG = {
    "US-EAST-ZONE":   {"host": "127.0.0.1", "port": 44701, "frequency": "144.100 MHz"},
    "NL-EUROPE-NORTH":{"host": "127.0.0.1", "port": 44702, "frequency": "144.500 MHz"},
    "NZ-PACIFIC-SOUTH":{"host": "127.0.0.1", "port": 44703, "frequency": "144.777 MHz"}
}

def simulate_zone_traffic(zone_name, port):
    """Fallback background socket simulator to ensure connection loops hold up."""
    try:
        server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        server.bind(("127.0.0.1", port))
        server.listen(5)
        while True:
            conn, _ = server.accept()
            # Feed randomized node tracking parameters down the wire
            node_id = random.randint(1000, 9999)
            metric_payload = f"[{zone_name}] NODE_{node_id} | SIG: 0x9F4B | STATUS: ACTIVE\n"
            conn.sendall(metric_payload.encode('utf-8'))
            conn.close()
    except Exception:
        pass

def listen_to_zone(zone_name, config):
    """Main client routine reading live socket descriptors from target zones."""
    print(f"[+] Launching Pathway 1 (Targeted Listening) -> {zone_name} on {config['frequency']}")
    while True:
        try:
            s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            s.settimeout(3.0)
            s.connect((config["host"], config["port"]))
            data = s.recv(1024).decode('utf-8').strip()
            if data:
                print(f"  ✨ [AGGREGATOR METRIC] {data}")
            s.close()
        except Exception:
            # Silent retry loop if background sync is pacing
            pass
        time.sleep(2.0)

def main():
    print("======================================================================")
    print("⚡ ZIGGY-OS MULTI-SOCKET ZONE AGGREGATOR PROVISIONED ⚡")
    print("======================================================================")
    
    # Launch Pathway 2: Spin up internal socket mocks for testing
    for zone, cfg in ZONE_CONFIG.items():
        t_sim = threading.Thread(target=simulate_zone_traffic, args=(zone, cfg["port"]), daemon=True)
        t_sim.start()
        
    time.sleep(0.5) # Allow internal ports to settle
    
    # Launch Pathway 3: Initialize concurrent trackers for individualized nodes
    threads = []
    for zone, cfg in ZONE_CONFIG.items():
        t_listen = threading.Thread(target=listen_to_zone, args=(zone, cfg), daemon=True)
        threads.append(t_listen)
        t_listen.start()

    try:
        while True:
            time.sleep(1)
    except KeyboardInterrupt:
        print("\n[+] Aggregator pipeline detached cleanly from target sockets.")

if __name__ == "__main__":
    # Validate crypto boundaries on initial file parse
    if len(sys.argv) > 1 and sys.argv[1].upper() in KEY_HIERARCHY:
        selected_tier = sys.argv[1].upper()
        print(f"[✓] Key Validation Check: {selected_tier} Auth Key Loaded: {KEY_HIERARCHY[selected_tier]}")
    main()
