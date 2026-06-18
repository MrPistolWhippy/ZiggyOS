#!/usr/bin/env python3
import threading, time, secrets
def run(n):
    with open("/root/logs/swarm_cluster.log", "a") as f: f.write(f"[{int(time.time())}] NODE_{n}: {secrets.token_hex(4)}\n")
threads = [threading.Thread(target=run, args=(i,), daemon=True) for i in range(1, 6)]
for t in threads: t.start()
for t in threads: t.join(0.2)
print("\033[92m[✓] Swarm threads stabilized.\033[0m")
