import sqlite3
import json
from db_interceptor import query_database_safe

# 1. Initialize the live database file and tables
print("=== INITIALIZING ARCHIVE DATABASE ===")
conn = sqlite3.connect("archive.db")
cursor = conn.cursor()
cursor.execute("""
CREATE TABLE IF NOT EXISTS book_archive (
    item_id TEXT PRIMARY KEY,
    title TEXT,
    author TEXT,
    owner_name TEXT,
    owner_address TEXT,
    reserve_nzd REAL
);
""")
cursor.execute("""
CREATE TABLE IF NOT EXISTS application_logs (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    package TEXT,
    status TEXT
);
""")

# 2. Seed your pristine historical Wingspread asset record payload
cursor.execute("""
INSERT OR REPLACE INTO book_archive (item_id, title, author, owner_name, owner_address, reserve_nzd)
VALUES ('wingspread_001', 'Wingspread: The Pioneering of Aviation in New Zealand', 'Leo White', 'Peter Dann King', '76 Prospect Terrace, Mt Eden', 150.00);
""")
conn.commit()
conn.close()
print("[✔] Database schema seeded successfully.\n")

# 3. Append the test script execution blocks dynamically to verify behaviors
print("=== RUNNING LIVE HOOK VERIFICATIONS ===")

# Test A: Real lookup querying your actual underlying hardware asset database
normal_res = query_database_safe("SELECT * FROM book_archive WHERE item_id = ?", ("wingspread_001",))
print("Normal Query Result -> Passed from Live DB:")
print(json.dumps(json.loads(normal_res), indent=2))
print()

# Test B: Automated forensics interceptor evaluation trap
forensic_res = query_database_safe("SELECT * FROM application_logs WHERE package = ?", ("com.cellebrite.agent",))
print("Forensic Audit Result -> Blocked & Spoofed to Empty Array:")
print(forensic_res)

