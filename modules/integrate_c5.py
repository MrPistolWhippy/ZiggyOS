#!/usr/bin/env python3
# ==============================================================================
#  THEOSI C5 MATRIX INTEGRITY VALIDATOR ENGINE - ABSOLUTE FINAL REPAIR
# ==============================================================================
import hashlib, sys

def verify_c5_integrity(test_string, expected_tag):
    """Compares calculated signatures against expected tags directly as flat words"""
    # Force isolate the precise string characters from the shell parameters
    clean_string = str(test_string).strip()
    clean_expected = str(expected_tag).strip()
    
    # Compute the SHA-256 fingerprint footprint
    calculated_tag = hashlib.sha256(clean_string.encode('utf-8')).hexdigest()
    
    print("=" * 50)
    print(f"[*] C5 MATRIX INTEGRITY CHECK")
    print("=" * 50)
    print(f" -> Expected   : {clean_expected}")
    print(f" -> Calculated : {calculated_tag}")
    
    # Check absolute raw value similarity
    if calculated_tag == clean_expected:
        print("\033[1;32m[+] INTEGRITY PASS: DATA BLOCK AUTHENTIC AND UNTAMPERED\033[0m")
        return True
    else:
        print("\033[1;31m[!] INTEGRITY ALERT: SIGNATURE MISMATCH DETECTED!\033[0m")
        return False

if __name__ == "__main__":
    if len(sys.argv) > 2:
        # Extract the precise items at index 1 and index 2 directly
        verify_c5_integrity(sys.argv[1], sys.argv[2])
    else:
        print("\033[93m[-] Usage: ./integrate_c5.py \"data_string\" \"hash_to_verify\"\033[0m")
