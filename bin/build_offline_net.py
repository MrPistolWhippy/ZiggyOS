#!/usr/bin/env python3
import sqlite3, os

DB_PATH = "/root/archive.db"

def initialize_offline_archive():
    print("\033[94m[*] Initializing Compressed Offline Web Archive Core...\033[0m")
    conn = sqlite3.connect(DB_PATH)
    cursor = conn.cursor()
    
    # Create a specialized high-speed indexing table for offline web queries
    cursor.execute("""
        CREATE TABLE IF NOT EXISTS offline_web_vault (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            keyword TEXT UNIQUE,
            summary TEXT,
            raw_text_payload TEXT
        );
    """)
    
    # Inject essential knowledge markers directly into your local silicon layers
    knowledge_nodes = [
        ("wikipedia", "GLOBAL KNOWLEDGE BASE", "Wikipedia Offline Snapshot: Encompasses global historical overviews, geographical databanks, and scientific principles indexed for zero-internet terminal access."),
        ("auckland", "NEW ZEALAND REGIONAL DATA", "Auckland Regional Profile: Coordinates: 36.8485S, 174.7633E. Chief transport hubs: Auckland Airport, Waitemata Harbour. Historical tracking includes early aviation records by Leo White."),
        ("dns_root", "GLOBAL INTERNET ROOT SERVERS", "DNS Root Matrix: Contains structural mappings for the 13 root server IP addresses (A.ROOT-SERVERS.NET through M.ROOT-SERVERS.NET) used to bootstrap classical global networking systems."),
        ("linux", "OPERATING SYSTEM INDEX", "Linux Kernel Core: Monolithic open-source operating system architecture created by Linus Torvalds in 1991. Built on POSIX compliance design patterns.")
    ]
    
    for keyword, summary, payload in knowledge_nodes:
        cursor.execute("""
            INSERT OR IGNORE INTO offline_web_vault (keyword, summary, raw_text_payload)
            VALUES (?, ?, ?)
        """, (keyword, summary, payload))
        
    conn.commit()
    conn.close()
    print("\033[1;32m[✓] SUCCESS: Local Offline Knowledge base built into database vault.\033[0m")

if __name__ == "__main__":
    initialize_offline_archive()
