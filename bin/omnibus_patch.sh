#!/bin/bash
echo -e "\033[1;35m[+] Running Mini-Omnibus Rank Patch...\033[0m"

# R1: tag_generator.py
cat << 'R1' > /root/modules/tag_generator.py
#!/usr/bin/env python3
import sys, hashlib
print(hashlib.sha256(" ".join(sys.argv[1:]).encode() if len(sys.argv)>1 else b"ZIGGYOS_DEFAULT").hexdigest())
R1

# R2: swarm_nested_active.py
cat << 'R2' > /root/modules/swarm_nested_active.py
#!/usr/bin/env python3
import threading, time, secrets
def run(n):
    with open("/root/logs/swarm_cluster.log", "a") as f: f.write(f"[{int(time.time())}] NODE_{n}: {secrets.token_hex(4)}\n")
threads = [threading.Thread(target=run, args=(i,), daemon=True) for i in range(1, 6)]
for t in threads: t.start()
for t in threads: t.join(0.2)
print("\033[92m[✓] Swarm threads stabilized.\033[0m")
R2

# R3: teal_cockpit.py
cat << 'R3' > /root/modules/teal_cockpit.py
#!/usr/bin/env python3
import os, sys
print("\033[2J\033[H\033[1;36mTHEOSI TEAL_COCKPIT LIVE MONITOR\033[0m\n"+"="*50)
if os.path.exists("/root/logs/network_sniff.log"):
    with open("/root/logs/network_sniff.log") as f: print("".join(f.readlines()[-8:]))
else: print("[-] Awaiting streams...")
R3

# R4: app_web.py
cat << 'R4' > /root/modules/app_web.py
#!/usr/bin/env python3
import http.server, socketserver, os
class HUDHandler(http.server.SimpleHTTPRequestHandler):
    def translate_path(self, path): return "/root/dashboard.html" if path == "/" else super().translate_path(path)
    def log_message(self, *a): return
socketserver.TCPServer.allow_reuse_address = True
try:
    with socketserver.TCPServer(("0.0.0.0", 8080), HUDHandler) as h:
        print("\033[1;32m[+] ZIGGYOS WEB HUD LIVE AT http://127.0.0.1:8080\033[0m"); h.serve_forever()
except Exception as e: print(e)
R4

chmod +x /root/modules/*.py
echo -e "\033[1;32m[+] OMNIBUS COMPLETE! All ranks rebuilt.\033[0m"
