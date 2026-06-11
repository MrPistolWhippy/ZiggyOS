import socket, sys, time, os

def run_binary_logger():
    print("=== [LOG ENGINE]: INITIALIZING BINARY STREAM LOG CONVERTER ===")
    print("  [+] Source Port    : UDP 4444 (Local Intercept Active)")
    print("  [+] Storage Path   : src/app/binary_broadcast_stream.bin")
    print("  [+] Anchor Vector  : 144.777 Flamescroll | Long Bay, NZ")
    
    # Bind to intercept the live broadcast packets locally
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    s.bind(("127.0.0.1", 4444))
    
    log_file = "src/app/binary_broadcast_stream.bin"
    print("=== [LOG ENGINE]: CONVERTING INCOMING PAYLOADS TO BINARY STREAM ===")
    
    count = 0
    try:
        while count < 4:
            data, addr = s.recvfrom(1024)
            payload_str = data.decode()
            timestamp = time.time()
            
            # Format packet data into a raw structural binary payload block
            binary_block = bytearray()
            binary_block.extend(int(timestamp).to_bytes(4, byteorder="big"))
            binary_block.extend(len(payload_str).to_bytes(2, byteorder="big"))
            binary_block.extend(data)
            
            with open(log_file, "ab") as f:
                f.write(binary_block)
                
            print(f"  [CONVERTED] Packet {count+1} | Bytes Logged: {len(binary_block)} | Key: p0w3r_0f_... -> SUCCESS")
            count += 1
            
        print(f"*** BINARY LOGGING PASS COMPLETE. CAPTURED {count} PACKETS NATIVELY. BRRRRPPP! ***")
    except KeyboardInterrupt:
        print("\n[-] Recording paused.")

if __name__ == '__main__':
    run_binary_logger()
