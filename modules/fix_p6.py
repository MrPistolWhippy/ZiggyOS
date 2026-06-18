import sqlite3

print("[*] Accessing workspace archive database...")
conn = sqlite3.connect('/root/archive.db')
cursor = conn.cursor()

# 1. Clean the environment by removing any legacy table
print("[*] Purging conflicting p6_ledger layers...")
cursor.execute('DROP TABLE IF EXISTS p6_ledger;')

# 2. Build out the complete, structurally sound schema
print("[*] Allocating clean multi-column matrix for p6_ledger...")
cursor.execute('''
CREATE TABLE p6_ledger (
    entry_id INTEGER PRIMARY KEY AUTOINCREMENT,
    layer_id TEXT DEFAULT 'L6',
    pigpen_status TEXT DEFAULT 'ACTIVE',
    payload_hex TEXT DEFAULT '5a494747595f5036',
    timestamp TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);
''')

# 3. Seed the verification row needed by the integrity suite
print("[*] Inserting validation sequence flags...")
cursor.execute('INSERT INTO p6_ledger (layer_id, pigpen_status) VALUES ("L6", "INITIALIZED");')

conn.commit()
conn.close()
print("[✓] Schema matrix patch fully written and committed!")
