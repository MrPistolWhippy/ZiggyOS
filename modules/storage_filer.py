#!/usr/bin/env python3
# ==============================================================================
#  THEOSI ZIGGYOS STORAGE RETENTION MANAGER & LEDGER INITIALIZER
# ==============================================================================
import os, sqlite3, secrets, sys, time

DB_PATH = "/root/archive.db"

def enforce_retention(target_layer=None, custom_payload=None):
    """Initializes standard baseline ledger blocks or injects dynamic tracking cells"""
    try:
        conn = sqlite3.connect(DB_PATH)
        cursor = conn.cursor()
        
        # Build the cryptographically secure system ledger index table layout
        cursor.execute("""
            CREATE TABLE IF NOT EXISTS system_index (
                record_hash TEXT PRIMARY KEY, 
                structural_layer INTEGER, 
                payload_hex TEXT, 
                epoch_timestamp INTEGER, 
                is_locked INTEGER
            );
        """)
        
        # Current POSIX Epoch Timestamp fallback handler for bare metal clocks
        current_epoch = int(time.time())
        
        if target_layer and custom_payload:
            # DYNAMIC INJECTION: Append fresh parameter tracking records on the fly
            record_hash = secrets.token_hex(16)
            payload_hex = custom_payload.encode('utf-8').hex()
            
            cursor.execute("""
                INSERT OR IGNORE INTO system_index VALUES (?, ?, ?, ?, 0)
            """, (record_hash, int(target_layer), payload_hex, current_epoch))
            print(f"\033[92m[✓] Dynamic Cell Injected -> Layer {target_layer} | Hash: {record_hash}\033[0m")
        else:
            # BASELINE INITIALIZATION: Populate initial data arrays if system is empty
            cursor.execute("SELECT COUNT(*) FROM system_index")
            if cursor.fetchone()[0] == 0:
                print("\033[94m[*] System ledger empty. Injecting cryptographically secure baseline layers...\033[0m")
                for l in range(1, 21):
                    record_hash = secrets.token_hex(16)
                    base_hex = f"5a49474759_{l}" # Matches hex footprint for "ZIGGY_"
                    cursor.execute("""
                        INSERT OR IGNORE INTO system_index VALUES (?, ?, ?, ?, 1)
                    """, (record_hash, l, base_hex, current_epoch))
                print("\033[92m[✓] Compact master system index initialized successfully.\033[0m")
                
        conn.commit()
        
        # Run storage maintenance sweep to compress structural space
        cursor.execute("VACUUM;")
        conn.close()
        
    except Exception as e:
        print(f"\033[91m[-] Storage Filer Collapse: {e}\033[0m")

if __name__ == "__main__":
    if len(sys.argv) > 2:
        # Accept execution inputs: ./storage_filer.py [layer_id] "[payload_text]"
        enforce_retention(sys.argv[1], sys.argv[2])
    else:
        enforce_retention()
