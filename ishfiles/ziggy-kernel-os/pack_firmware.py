import struct
import os

def pack_bin():
    input_file = "ziggyos.bin"
    output_file = "ziggyos.ppk"
    
    if not os.path.exists(input_file):
        print("Error: ziggyos.bin missing!")
        return

    # 1. Gather file sizing details
    bin_size = os.path.getsize(input_file)
    
    # 2. Structure the Header Prefix
    # Magic: 'ZIGG' | Target: 0x4330 | Load Address: 0x14000000 | Payload Size
    magic = b"ZIGG"
    target_id = 0x4330
    load_addr = 0x14000000
    
    # Standard 32-bit unsigned integer formats (I) used exclusively
    header = struct.pack("<4sIII", magic, target_id, load_addr, bin_size)
    
    # 3. Read raw payload binary bytes
    with open(input_file, "rb") as f:
        payload = f.read()
        
    # 4. Compile packed deployment bundle 
    with open(output_file, "wb") as f:
        f.write(header + payload)
        
    print(f"[SUCCESS] Pack completed -> {output_file} ({len(header) + bin_size} bytes)")

if __name__ == "__main__":
    pack_bin()
