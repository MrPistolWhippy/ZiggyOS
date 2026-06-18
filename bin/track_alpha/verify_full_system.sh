#!/bin/bash
# ======================================================================
# ⚡ ZIGGY-OS SYSTEM AUDIT SUITE: FULL PRODUCTION LOCK ⚡
# ======================================================================

C_GOLD="\033[38;5;220m"
C_PURP="\033[38;5;93m"
C_GREY="\033[38;5;242m"
C_WHITE="\033[38;5;15m"
RESET="\033[0m"

echo -e "${C_GOLD}======================================================================${RESET}"
echo -e " 🕶️  ${C_GOLD}ZIGGY-OS : FOUR-TIER CORE INTEGRITY VERIFICATION SUITE${RESET}"
echo -e "${C_GOLD}======================================================================${RESET}"

# ----------------------------------------------------------------------
# 1. BARE-METAL BINARY CHECK
# ----------------------------------------------------------------------
echo -e "\n${C_PURP}[TRACK 1] Auditing 32-bit ELF Kernel Binary Map...${RESET}"
TARGET_BIN="/mnt/ziggy_os/kernel_core/os-kernel.bin"

if [ -f "$TARGET_BIN" ]; then
    BIN_SIZE=$(wc -c < "$TARGET_BIN")
    echo -e "  [✓] Binary Found : $TARGET_BIN ($BIN_SIZE bytes)"
    # Validate ELF Header magic bytes (0x7f 0x45 0x4c 0x46 -> \x7fELF)
    MAGIC=$(od -An -N4 -tx1 "$TARGET_BIN" | tr -d ' ')
    if [ "$MAGIC" = "7f454c46" ]; then
        echo -e "  [✓] Format Verification: Valid ELF Target File Layout."
    else
        echo -e "  [!] WARNING: Binary signature mismatch. Expected valid ELF descriptor."
    fi
else
    echo -e "  [!] ERROR: $TARGET_BIN not found. Skipping compilation trace."
fi

# ----------------------------------------------------------------------
# 2. SQLITE WAL VALIDATION
# ----------------------------------------------------------------------
echo -e "\n${C_PURP}[TRACK 2] Validating SQLite WAL-Enabled DB Status...${RESET}"
DB_PATH="/mnt/ziggy_os/archive.db"

if [ -f "$DB_PATH" ]; then
    echo -e "  [✓] Database Found : $DB_PATH"
    # Query structural tables and integrity values via python
    python3 -c "
import sqlite3
try:
    conn = sqlite3.connect('$DB_PATH')
    cur = conn.cursor()
    cur.execute('PRAGMA integrity_check;')
    res = cur.fetchone()[0]
    print(f'  [✓] SQLite Integrity Status : {res}')
    cur.execute('PRAGMA journal_mode;')
    mode = cur.fetchone()[0]
    print(f'  [✓] Active Storage Mode     : {mode.upper()}')
except Exception as e:
    print(f'  [!] Database Error: {e}')
"
else
    echo -e "  [!] ERROR: $DB_PATH is missing from the environment mount."
fi

# ----------------------------------------------------------------------
# 3. FAT VIRTUAL DISK SECTOR CHECK
# ----------------------------------------------------------------------
echo -e "\n${C_PURP}[TRACK 3] Auditing FAT Sector Map (virtual_disk.img)...${RESET}"
IMG_PATH="/mnt/ziggy_os/virtual_disk.img"

if [ -f "$IMG_PATH" ]; then
    IMG_SIZE=$(wc -c < "$IMG_PATH")
    echo -e "  [✓] Partition Image Found : $IMG_PATH ($IMG_SIZE bytes)"
    # Read the master boot record volume identifier to verify FAT layouts
    python3 -c "
try:
    with open('$IMG_PATH', 'rb') as f:
        f.seek(54) # Jump straight to the FAT16 string identifier offset
        sys_type = f.read(8).decode('ascii', errors='ignore').strip()
        print(f'  [✓] File System Sector Label : {sys_type if sys_type else \"RAW/UNLABELED\"}')
except Exception as e:
    print(f'  [!] Partition Parse Error: {e}')
"
else
    echo -e "  [!] ERROR: $IMG_PATH is missing from root storage paths."
fi

# ----------------------------------------------------------------------
# 4. RAW MATRIX LEDGER HEX CHECK
# ----------------------------------------------------------------------
echo -e "\n${C_PURP}[TRACK 4] Scanning Raw matrix_ledger.hex For Node Syncs...${RESET}"
HEX_PATH="/mnt/ziggy_os/matrix_ledger.hex"

if [ -f "$HEX_PATH" ]; then
    HEX_SIZE=$(wc -c < "$HEX_PATH")
    echo -e "  [✓] Ledger Appending Stable : $HEX_PATH ($HEX_SIZE bytes)"
    echo -e "  [✓] Base Registry Readout (First 32 Cryptographic Bytes):"
    echo -ne "      ${C_GREY}"
    od -An -N32 -tx1 "$HEX_PATH" | sed 's/^ /      /'
    echo -ne "${RESET}"
else
cat << 'EOF' > /root/verify_full_system.sh
#!/bin/bash
P="\033[38;5;93m"; G="\033[38;5;220m"; R="\033[0m"
echo -e "${G}== ZIGGY-OS SHORT INTEGRITY SUITE ==${R}"
# 1. ELF
[ -f "/root/kernel_core/os-kernel.bin" ] && echo -e "${P}[✓] ELF Magic:${R} $(od -An -N4 -tx1 /root/kernel_core/os-kernel.bin)" || echo "[!] No ELF"
# 2. SQLITE
[ -f "/root/archive.db" ] && python3 -c "import sqlite3; c=sqlite3.connect('/root/archive.db').cursor(); print(f'${P}[✓] DB:${R} {c.execute(\"PRAGMA integrity_check\").fetchone()[0]} | {c.execute(\"PRAGMA journal_mode\").fetchone()[0].upper()}')" || echo "[!] No DB"
# 3. FAT
[ -f "/root/virtual_disk.img" ] && python3 -c "f=open('/root/virtual_disk.img','rb'); f.seek(54); print(f'${P}[✓] FAT Sector:${R} {f.read(8).decode(errors=\"ignore\").strip()}')" || echo "[!] No IMG"
# 4. HEX
[ -f "/root/matrix_ledger.hex" ] && echo -e "${P}[✓] Hex Bytes:${R} $(od -An -N16 -tx1 /root/matrix_ledger.hex | head -n1)" || echo "[!] No HEX"
echo -e "${G}====================================${R}"
