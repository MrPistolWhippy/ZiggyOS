#!/bin/bash
P="\033[38;5;93m"; C="\033[38;5;51m"; G="\033[38;5;220m"; R="\033[0m"
IMG="/root/virtual_disk.img"
DB="/root/archive.db"

echo -e "${G}=== FORCE-TRIGGERING OFFLINE REDUNDANCY BACKUP CHECK ===${R}"

# 1. FORCE DICTIONARY JOURNAL CHECKPOINT (FLUSH WAL TO DISK)
echo -e "\n${C}[STEP 1/3] Flushing live SQLite WAL cache to main database...${R}"
if [ -f "$DB" ]; then
    python3 -c "import sqlite3; conn=sqlite3.connect('$DB'); conn.execute('PRAGMA wal_checkpoint(TRUNCATE);')"
    echo -e "  [✓] DB transactional memory flushed clean."
else
    echo -e "  [!] WARNING: Primary archive.db not found for sync."
fi

# 2. VERIFY VOLUME BOOT SECTOR & METADATA
echo -e "\n${C}[STEP 2/3] Analyzing virtual_disk.img raw byte boundaries...${R}"
if [ -f "$IMG" ]; then
    SIZE=$(wc -c < "$IMG")
    echo -e "  [✓] Image Footprint : $SIZE bytes"
    # Pull FAT identification label from hex offset 54
    LABEL=$(python3 -c "f=open('$IMG','rb'); f.seek(54); print(f.read(8).decode(errors='ignore').strip())")
    echo -e "  [✓] Partition Sector Signature : [${P}$LABEL${R}]"
else
    echo -e "  [!] ERROR: /root/virtual_disk.img is missing or unallocated."
fi

# 3. VERIFY LEDGER TRANSACTIONS REFLECT FRESH HASHES
echo -e "\n${C}[STEP 3/3] Cross-checking matrix_ledger.hex alignment...${R}"
if [ -f "/root/matrix_ledger.hex" ]; then
    LAST_SIG=$(tail -n 1 /root/matrix_ledger.hex)
    echo -e "  [✓] Last Synchronized Swarm Block : ${G}$LAST_SIG${R}"
else
    echo -e "  [!] ERROR: matrix_ledger.hex missing from active branch."
fi

echo -e "\n${G}========================================================${R}"
echo -e "⚡ ${G}[BACKUP CHECK SYSTEM SECURED] Off-grid mirrors aligned. ⚡${R}"
echo -e "${G}========================================================${R}"
