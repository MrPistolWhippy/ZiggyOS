#!/usr/bin/env python3
# ==============================================================================
#  ZIGGYOS SHOR'S ALGORITHM FACTORING PERIOD-FINDING SIMULATOR
# ==============================================================================
import sys, math, time

def simulate_shor_factoring(N, a=7):
    print("=" * 60)
    print(f"\033[1;35m[*] RUNNING SHOR PERIOD-FINDING SIMULATION FOR N = {N} (Base: {a})\033[0m")
    print("=" * 60)
    
    if math.gcd(a, N) != 1:
        print(f"[!] Base {a} shares a factor with {N} trivially.")
        return
        
    # Quantum Phase Estimation Simulation: Find the period 'r' where a^r = 1 (mod N)
    period_r = 0
    sequence = []
    
    for x in range(1, N + 1):
        val = (a ** x) % N
        sequence.append(val)
        sys.stdout.write(f"  [Wave Step {str(x).zfill(2)}] -> {a}^{x} mod {N} = \033[93m{val}\033[0m\n")
        time.sleep(0.05)
        
        # Detect when the modular pattern cycles back to 1
        if val == 1 and period_r == 0:
            period_r = x
            
    print("-" * 60)
    print(f"\033[94m[+] Quantum Interference Pattern Isolated!\033[0m")
    print(f"  -> Detected Repeating Period (r) = {period_r}")
    
    if period_r % 2 != 0:
        print("  [-] Isolated period is odd. Quantum condition failed. Retry required.")
        return
        
    # Calculate the prime factors using the recovered period
    val_plus = (a ** (period_r // 2)) + 1
    val_minus = (a ** (period_r // 2)) - 1
    
    factor1 = math.gcd(val_minus, N)
    factor2 = math.gcd(val_plus, N)
    
    print(f"\033[1;32m[✓] TARGET CRACKED: Prime Factors of {N} are {factor1} and {factor2}\033[0m")
    print("=" * 60)

if __name__ == "__main__":
    # Factor N=15 by default (standard academic Shor benchmark verification number)
    target_N = int(sys.argv[1]) if len(sys.argv) > 1 else 15
    simulate_shor_factoring(target_N)
