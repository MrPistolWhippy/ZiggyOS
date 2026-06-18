#!/usr/bin/env python3
import socket, threading, sys, os
from datetime import datetime
from cipher_test import calculate_ciphers

PORTS = [80, 443, 44777]
os.makedirs("/root/logs", exist_ok=True)

def log(data, addr, proto, port):
    ts = datetime.now().strftime('%H:%M:%S')
    hex_p = data.hex()[:12]
    ascii_str = "".join([chr(b) if 32 <= b <= 126 else "." for b in data[:10]])
    
    # Calculate unique Alphanumeric Fingerprints on the fly
    ord_s, red_s, kay_s = calculate_ciphers(ascii_str)
    checksum = f"O:{ord_s}|R:{red_s}|K:{kay_s}"
    
    msg = f"[{ts}] {proto} -> P{port} | HEX: {hex_p} | TXT: {ascii_str} | FP: {checksum}\n"
    with open("/root/logs/network_sniff.log", "a") as f: f.write(msg)
    print(f"\033[92m[+] INTERCEPTED {proto} (FP: {checksum})\033[0m")

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
        except: pass

print(f"\033[94m[*] Network Fingerprint Matrix Online on ports: {PORTS}\033[0m")
for p in PORTS:
    threading.Thread(target=listen_udp, args=(p,), daemon=True).start()
    threading.Thread(target=listen_tcp, args=(p,), daemon=True).start()
while True: pass
