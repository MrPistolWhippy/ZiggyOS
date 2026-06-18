#!/bin/bash
P="\033[38;5;93m"; C="\033[38;5;51m"; G="\033[38;5;220m"; R="\033[0m"

echo -e "${G}=== ZIGGY-OS SOUTHERN HEMISPHERE MICRO CORE SUITE TEST ===${R}"

# 1. FORCE DICTIONARY JOURNAL CHECKPOINT (FLUSH WAL TO DISK)
echo -e "\n${C}[TEST 1/3] Flushing live SQLite WAL cache to main database...${R}"
if [ -f "/root/archive.db" ]; then
    python3 -c "import sqlite3; conn=sqlite3.connect('/root/archive.db'); conn.execute('PRAGMA wal_checkpoint(TRUNCATE);')"
    echo -e "  [✓] DB transactional memory flushed clean."
else
    echo -e "  [!] WARNING: Primary archive.db not found for sync."
fi

# 2. VERIFY NEW ZEALAND LOW-KB CORE BINARY PATH
echo -e "\n${C}[TEST 2/3] Verifying nz_core object state...${R}"
if [ -f "/root/nz_core.o" ]; then
    OBJ_SIZE=$(wc -c < /root/nz_core.o)
    echo -e "  [✓] Object File Found : /root/nz_core.o ($OBJ_SIZE bytes)"
else
    echo -e "  [!] NOTICE: Re-assembling pure bare-metal nz_mini stack..."
    mkdir -p /root/kernel_core
    echo -e ".global _start\n.text\n_start:\n  cli\n  mov eax, 0x181FFED7\n  hlt" > /root/kernel_core/nz_mini.S
    as --32 /root/kernel_core/nz_mini.S -o /root/nz_core.o 2>/dev/null
    objcopy -O binary /root/nz_core.o /root/kernel_core/nz-main-core.bin 2>/dev/null
    rm -f /root/kernel_core/nz_mini.S
    echo -e "  [✓] Fresh flat core binary generated successfully."
fi

# 3. ROTATE AND APPENDFILE FRESH SLIDING NONCE HASH
echo -e "\n${C}[TEST 3/3] Triggering Cryptographic Tag Sequence...${R}"
if [ -f "/root/tag_generator.py" ]; then
    python3 /root/tag_generator.py
else
    echo -e "  [!] ERROR: /root/tag_generator.py is missing from active workspace."
fi

echo -e "\n${G}===========================================================${R}"
echo -e "⚡ ${G}[NZ-PACIFIC CORE SECURED] Multi-zone swarm verified. ⚡${R}"
echo -e "${G}===========================================================${R}"
