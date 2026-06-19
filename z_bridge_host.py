#!/usr/bin/env python3
# ---------------------------------------------------------------------
# NETWORK ENGINE: UDP ASYNCHRONOUS SWARM LISTENER INTERFACE
# ---------------------------------------------------------------------
import socket
import sys

# Define standard network communication vectors
LISTEN_IP = "0.0.0.0"  # Bind universally to all local network interfaces
LISTEN_PORT = 7003     # Custom secure channel designated for node P20 transfers

# Initialize raw UDP socket configuration
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

try:
    sock.bind((LISTEN_IP, LISTEN_PORT))
    print(f"[*] BRIDGE: Active network socket daemon listening on UDP://{LISTEN_IP}:{LISTEN_PORT}")
    print("[*] PRESS [CTRL+C] to cleanly terminate the listener pool thread context.")
    print("---------------------------------------------------------------------")
    
    while True:
        # Halt execution path asynchronously until an incoming packet strikes the buffer
        data, addr = sock.recvfrom(1024)  # 1KB Data packet chunk limitation barrier
        print(f"[+] INCOMING PACKET RECEIVED FROM PEER LINK: {addr[0]}:{addr[1]}")
        print(f"    Payload Stream Size : {len(data)} bytes")
        print(f"    Raw Hexadecimal dump: {data.hex()[:32]}...")

except KeyboardInterrupt:
    print("\n[!] Bridge communication listening thread suspended cleanly.")
    sys.exit(0)
except Exception as e:
    print(f"\n[ERROR] Critical socket infrastructure fault: {e}")
    sys.exit(1)
