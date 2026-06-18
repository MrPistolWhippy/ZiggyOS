import os, sqlite3, secrets
DB_PATH = "/root/archive.db"
def enforce_retention():
    if not os.path.exists("/root"): os.makedirs("/root", exist_ok=True)
    conn = sqlite3.connect(DB_PATH)
    cursor = conn.cursor()
    cursor.execute("CREATE TABLE IF NOT EXISTS system_index(record_hash TEXT PRIMARY KEY, structural_layer INTEGER, payload_hex TEXT, epoch_timestamp INTEGER, is_locked INTEGER);")
    cursor.execute("SELECT COUNT(*) FROM system_index")
    if cursor.fetchone()[0] == 0:
        for l in range(1, 21):
            cursor.execute("INSERT OR IGNORE INTO system_index VALUES(?, ?, ?, (strftime('%s','now')), 1)", (secrets.token_hex(16), l, f"5a49474759_{l}"))
    conn.commit()
    cursor.execute("VACUUM;")
    conn.close()
    print("[✓] Compact index initialized successfully.")
if __name__ == "__main__": enforce_retention()
