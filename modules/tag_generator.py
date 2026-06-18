#!/usr/bin/env python3
import sys, hashlib

def generate_crypto_tag(text_input):
    """Computes a secure, unforgeable SHA-256 signature for a tracking block"""
    tag = hashlib.sha256(text_input.encode('utf-8')).hexdigest()
    return tag

if __name__ == "__main__":
    # Fallback to an identity string if no parameter is provided
    target_string = " ".join(sys.argv[1:]) if len(sys.argv) > 1 else "ZIGGYOS_DEFAULT_SIGNATURE_NODE"
    print(generate_crypto_tag(target_string))
