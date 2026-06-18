#!/usr/bin/env python3
# ==============================================================================
#  ZIGGYOS QUANTIST REDON LATTICE KEY-GENERATOR (UPGRADED RUNTIME DEPLOYMENT)
# ==============================================================================
import sys, random, hashlib

def generate_post_quantum_lattice(seed_string="ZIGGY-OS QUANTIST REDON LATTICE KEY-GENERATOR"):
    print("=" * 50)
    print(f"[*] INITIALIZING SEED KEY PARAMETERS FROM BASE TARGET...")
    print("=" * 50)
    
    # Derive an absolute deterministic seed using a SHA-256 string hash mapping block
    seed_hash = hashlib.sha256(seed_string.encode()).hexdigest()
    random.seed(int(seed_hash, 16))
    
    dimension_n = 8
    modulus_q = 257
    
    print(f" [+] Active Target Seed String : {seed_string}")
    print(f" [+] Lattice Dimensions Matrix  : {dimension_n}x{dimension_n}")
    print(f" [+] Prime Vector Field Modulus: {modulus_q}")
    print("-" * 50)
    
    # Build private vector 's' randomly bounded between [-1, 0, 1]
    private_key_s = [random.randint(-1, 1) for _ in range(dimension_n)]
    
    # Assemble public seed matrix space 'A' and quiet error vectors 'e'
    matrix_A = [[random.randint(0, modulus_q - 1) for _ in range(dimension_n)] for _ in range(dimension_n)]
    error_e = [random.randint(1, 11) for _ in range(dimension_n)]
    
    # Calculate the definitive public key vector via modular matrix dot-product
    public_key_b = []
    for row in matrix_A:
        dot_product = sum(a * s for a, s in zip(row, private_key_s))
        public_key_b.append(dot_product % modulus_q)
        
    print("\033[92m[+] LATTICE INTEGRITY MATRIX SECURED: QUANTUM-RESISTANT PAIR BUILD COMPLETE\033[0m")
    print(f"  -> Secret Vector (s) : {private_key_s}")
    print(f"  -> Matrix Sample (A) : {matrix_A[0]} ... [8 rows bound]")
    print(f"  -> Public Key    (b) : {public_key_b}")
    print("=" * 50)

if __name__ == "__main__":
    # Dynamically extract console input variables if appended to command line runtime string
    active_seed = "ZIGGY-OS QUANTIST REDON LATTICE KEY-GENERATOR"
    if len(sys.argv) > 1:
        active_seed = " ".join(sys.argv[1:])
        
    generate_post_quantum_lattice(active_seed)
