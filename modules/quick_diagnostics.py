#!/usr/bin/env python3
import sys, os, time, hashlib, math

def run_ziggyos_sweep():
    print("\033[38;5;198m============================================================\033[0m")
    print("\033[1;36m           ZIGGYOS CORE SILICON INTEGRITY RECON             \033[0m")
    print("\033[38;5;198m============================================================\033[0m")
    
    # 1. STRUCTURAL VERIFICATION: Audit filesystem layers
    paths = ["/root/bin", "/root/modules", "/root/config", "/root/logs"]
    print("\033[94m[*] Phase 1: Checking Layer-2 Directory Matrix Topology...\033[0m")
    for p in paths:
        status = "\033[92m[ FOUND ]\033[0m" if os.path.exists(p) else "\033[91m[ MISSING ]\033[0m"
        print(f"  -> Path: {p.ljust(15)} {status}")
    
    # 2. CRYPTO VERIFICATION: Test SHA-256 fingerprint alignments
    print("\n\033[94m[*] Phase 2: Testing Alphanumeric Gematria Hashing Gates...\033[0m")
    test_str = "THEOSI_NODE_1"
    computed_hash = hashlib.sha256(test_str.encode()).hexdigest()
    print(f"  -> Input Payload : {test_str}")
    print(f"  -> Generated Tag : \033[1;32m{computed_hash[:32]}...\033[0m")
    
    # 3. MATH SIMULATION: Run Shor period-finding cycle checks for N=15
    print("\n\033[94m[*] Phase 3: Executing Shor Factor Wave Wavelength Scan...\033[0m")
    N, a, period_r = 15, 7, 0
    for x in range(1, N + 1):
        if (a ** x) % N == 1:
            period_r = x
            break
    f1 = math.gcd((a ** (period_r // 2)) - 1, N)
    f2 = math.gcd((a ** (period_r // 2)) + 1, N)
    print(f"  -> Target Value  : N = {N}")
    print(f"  -> Period Found  : r = {period_r}")
    print(f"  -> Keys Cracked  : \033[1;32m[{f1} and {f2}]\033[0m")
    print("\033[38;5;198m============================================================\033[0m")

if __name__ == "__main__":
    run_ziggyos_sweep()
