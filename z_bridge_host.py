import socket, sys
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
try:
    sock.bind(("0.0.0.0", 7003))
    print("[*] BRIDGE ACTIVE ON UDP://0.0.0.0:7003. PRESS CTRL+C TO EXIT.")
    while True:
        data, addr = sock.recvfrom(1024)
        print(f"[+] PACKET FROM {addr}: {len(data)} BYTES // HEX: {data.hex()[:16]}")
except KeyboardInterrupt: sys.exit(0)
