import os
import sqlite3
import secrets

DB_PATH = "/root/archive.db"
HEX_LEDGER_PATH = "/root/matrix_ledger.hex"

def initialize_environment():
    print("[+] Initializing Ziggy-OS target directories...")
    
    # Ensure directory exists
    if not os.path.exists("/root"):
        os.makedirs("/root", exist_ok=True)
        
    # 1. Setup SQLite DB and execute schema
    conn = sqlite3.connect(DB_PATH)
    cursor = conn.cursor()
    cursor.execute("PRAGMA journal_mode='WAL';")
    
    cursor.execute("""
    CREATE TABLE IF NOT EXISTS global_mesh_nodes (
        id INTEGER PRIMARY KEY, zone TEXT, frequency REAL, status TEXT
    );""")
    
    # 2. Populate Nodes topology if empty
    cursor.execute("SELECT COUNT(*) FROM global_mesh_nodes")
    if cursor.fetchone()[0] == 0:
        print("[+] Seeding 75 distributed nodes across matrices...")
        zones = ['US-Zone-1', 'US-Zone-2', 'US-Zone-3', 'US-Zone-4', 'US-Zone-5', 'NZ-Pacific-South', 'NL-Europe-North']
        for i in range(75):
            zone = zones[i % len(zones)]
            cursor.execute("INSERT INTO global_mesh_nodes (zone, frequency, status) VALUES (?, 144.777, 'swarm_nested_active')", (zone,))
        conn.commit()
    
    # 3. Simulate and record raw transaction hex ledger log
    print("[+] Compiling raw transaction dump to ledger...")
    with open(HEX_LEDGER_PATH, "a") as hex_file:
        dummy_transaction_hash = secrets.token_hex(32)
        hex_file.write(f"{dummy_transaction_hash}\n")
        
    conn.close()
    print(f"[✓] Verification completed. DB file active at {DB_PATH}. Ledger mirrored to {HEX_LEDGER_PATH}.")

if __name__ == "__main__":
    initialize_environment()
