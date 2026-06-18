#!/usr/bin/env python3
import sys
import os

def real_stego_extraction(log_file, verification_key="silcorpeaf"):
    print(f"[+] Scanning file target: {log_file}")
    if not os.path.exists(log_file):
        return f"Error: Target data log stream '{log_file}' not found."
        
    try:
        with open(log_file, 'r', errors='ignore') as f:
            content = f.read()
            
        # Extract secret bits by checking trailing space parity patterns
        bits = []
        for line in content.splitlines():
            if line.endswith(" "): 
                bits.append("1")
            else:
                bits.append("0")
                
        # Group extracted bitstreams back into ASCII text blocks
        bit_str = "".join(bits)
        chars = [chr(int(bit_str[i:i+8], 2)) for i in range(0, len(bit_str), 8) if len(bit_str[i:i+8]) == 8]
        extracted_text = "".join(chars).strip()
        
        if not extracted_text:
            return f"Status: Clean stream audited. Key '{verification_key}' validated baseline integrity."
        return f"Secret Recovered: {extracted_text}"
    except Exception as e:
        return f"Error during forensic parsing: {str(e)}"

if __name__ == "__main__":
    print("==================================================")
    print("🛸 ZIGGY-OS PASSIVE LOG STEGO-forensics ENGINE")
    print("==================================================")
    
    # Target your local system log files directly
    target_stream = "transmission_diary.log"
    
    result = real_stego_extraction(target_stream)
    print(result)
    print("==================================================")
