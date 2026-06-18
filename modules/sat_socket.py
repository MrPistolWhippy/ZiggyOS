#!/usr/bin/env python3
import socket, threading, sys
from datetime import datetime

PORTS = [80, 443, 44777]

def log(data, addr, proto, port):
    ts = datetime.now().strftime('%H:%M:%S.%f')[:-3]
    msg = f"[{ts}] {proto} {addr[0]}:{addr[1]} -> PORT {port} | HEX: {data.hex()[:30]}\n"
    with open("/root/logs/network_sniff.log", "a") as f: f.write(msg)
    print(f"\033[92m[+] INTERCEPTED {proto} ON PORT {port} ({len(data)} Bytes)\033[0m")

def listen(port, is_udp):
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM if is_udp else socket.SOCK_STREAM)
    if not is_udp: s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    try:
        s.bind(("0.0.0.0", port))
        if not is_udp: s.listen(5)
        while True:
            data, addr = s.recvfrom(65535) if is_udp else s.accept()[0].recv(65535), s.accept()[1] if not is_udp else None
            if data: log(data, addr or ("0.0.0.0", 0), "UDP" if is_udp else "TCP", port)
    except: pass

print(f"\033[94m[*] Listening Promiscuously on ports: {PORTS}\033[0m")
for p in PORTS:
    threading.Thread(target=listen, args=(p, True), daemon=True).start()
    threading.Thread(target=listen, args=(p, False), daemon=True).start()
try:
    while True: pass
except KeyboardInterrupt: print("\n[-] Detached.")
