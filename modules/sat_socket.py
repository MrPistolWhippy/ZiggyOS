#!/usr/bin/env python3
import socket, threading, sys, os
from datetime import datetime

PORTS = [80, 443, 44777]
os.makedirs("/root/logs", exist_ok=True)

def log(data, addr, proto, port):
    ts = datetime.now().strftime('%H:%M:%S')
    hex_payload = data.hex()[:16]
    
    # Translate raw packet bytes directly into clean ASCII text strings
    ascii_str = "".join([chr(b) if 32 <= b <= 126 else "." for b in data[:12]])
    
    msg = f"[{ts}] {proto} -> PORT {p} | HEX: {hex_payload} | TXT: {ascii_str}\n"
    with open("/root/logs/network_sniff.log", "a") as f: f.write(msg)
    print(f"\033[92m[+] INTERCEPTED {proto} ON PORT {port} ({len(data)} B)\033[0m")

def listen_udp(port):
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    s.bind(("0.0.0.0", port))
    while True:
        data, addr = s.recvfrom(65535)
        if data: log(data, addr, "UDP", port)

def listen_tcp(port):
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    s.bind(("0.0.0.0", port))
    s.listen(5)
    while True:
        try:
            conn, addr = s.accept()
            data = conn.recv(65535)
            if data: log(data, addr, "TCP", port)
            conn.close()
        except Exception as e: pass

print(f"\033[94m[*] Listening Promiscuously on ports: {PORTS}\033[0m")
for p in PORTS:
    threading.Thread(target=listen_udp, args=(p,), daemon=True).start()
    threading.Thread(target=listen_tcp, args=(p,), daemon=True).start()

try:
    while True: pass
except KeyboardInterrupt: print("\n[-] Detached.")
