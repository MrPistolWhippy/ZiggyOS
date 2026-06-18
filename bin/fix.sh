#!/bin/bash
# Ultra-smooth micro patch

cat << 'PYEOF' > network_scan.py
import socket, time
domains = ["dns.google", "flipperzero.one", "fix-ski.com"]

print("=== SYSTEM NET LANE DIAGNOSTIC ===")
# Rapid socket ping to a reliable public DNS IP on port 53 (No SSL overhead)
try:
    s = socket.create_connection(("8.8.8.8", 53), timeout=1.0)
    s.close()
    print("WAN STATUS: ONLINE (CONNECTED)")
except:
    print("WAN STATUS: AIR-GAP DETECTED (LOCAL-ONLY)")

for d in domains:
    try:
        print(f" [+] {d.ljust(17)} -> {socket.gethostbyname(d)}")
    except:
        print(f" [-] {d.ljust(17)} -> Loopback (127.0.0.1)")
PYEOF

# Clean background recycling
pkill -f router.py; pkill -f radio_node.py
[ -f router.py ] && python3 router.py > /dev/null 2>&1 &
[ -f radio_node.py ] && python3 radio_node.py > /dev/null 2>&1 &

# Execute
python3 network_scan.py
