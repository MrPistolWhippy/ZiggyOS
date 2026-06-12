#!/usr/bin/env python3
import sys

def decrypt_hex_and_shift(hex_str, shift=-1):
    try:
        # Step 1: Decode hex directly to ASCII text
        ascii_text = bytes.fromhex(hex_str).decode('utf-8', errors='replace')
        
        # Step 2: Apply alphabetical Caesar shift
        final_str = ""
        for char in ascii_text:
            if char.isalpha():
                start = ord('a') if char.islower() else ord('A')
                final_str += chr((ord(char) - start + shift) % 26 + start)
            else:
                final_str += char
        return final_str
    except Exception as e:
        return f"Error: {str(e)}"

if __name__ == "__main__":
    print("==================================================")
    print("🔮 ZIGGY-OS ADVANCED MULTI-LAYER DECRYPTER")
    print("==================================================")
    
    test_payload = "746a6d64707371666267" 
    
    print(f"[+] Ingesting Raw Hex Stream: {test_payload}")
    result = decrypt_hex_and_shift(test_payload, shift=-1)
    print(f"[+] Decrypted True Output   : {result}")
    print("==================================================")
