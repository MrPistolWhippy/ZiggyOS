import struct

def encode_satellite_matrix(freq_str, coordinate_str):
    print(f"[*] Packaging Network Matrix: {freq_str} @ {coordinate_str}")
    
    try:
        # Extract the decimal components of your frequency
        freq_major, freq_minor = map(int, freq_str.split('.'))
        
        # Extract the custom oversized coordinate array
        octets = map(int, coordinate_str.split('.'))
        
        # Pack everything into a tight, bare-metal binary frame structure
        # H = unsigned short, I = unsigned int
        binary_frame = struct.pack('!H H I I I I', freq_major, freq_minor, *octets)
        
        print(f"[+] Hex Frame Compiled: {binary_frame.hex().upper()}")
        return binary_frame
    except Exception as e:
        print(f"[-] Encoding failed: {e}")

encode_satellite_matrix("144.777", "999.999.999.432")
