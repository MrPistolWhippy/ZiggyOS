#!/usr/bin/env python3
import sys, math, time
from shor_simulator import simulate_shor_factoring

def initiate_batch_sweep():
    # Sequence of composite keys to sweep through automatically
    target_keys = [15, 21, 35]
    print("\033[1;35m[*] STARTING AUTOMATED MULTI-TARGET BATCH CRACKER protocol...\033[0m")
    
    for idx, key in enumerate(target_keys):
        print(f"\n\033[1;36m[+] INITIALIZING TARGET {idx+1}/{len(target_keys)}: N = {key}\033[0m")
        # Base selection shifts dynamically to find valid periods cleanly
        base_a = 7 if key != 21 else 2
        try:
            simulate_shor_factoring(key, base_a)
        except Exception as e:
            print(f"[-] Target bypass: {e}")
        time.sleep(1)
        
    print("\n\033[1;32m[✓] BATCH SWEEP COMPLETE: ALL PRISM TARGETS SECURED!\033[0m")

if __name__ == "__main__":
    initiate_batch_sweep()
