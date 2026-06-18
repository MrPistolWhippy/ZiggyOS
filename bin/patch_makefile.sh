#!/bin/ash
set -e

echo "=== TRANSLATING DATA ARCHIVE TO C COMPATIBLE SOURCE ==="

# 1. Generate the direct C implementation file
cat << 'SRC_EOF' > z_os/src/archive.c
#include "archive.h"

// Hardcoded static struct placed explicitly into page-aligned .rodata space
const HistoricalRecord __attribute__((section(".rodata"))) wingspread_archive = {
    .item_id = "wingspread_1941_archive_001",
    .title = "Wingspread: The Pioneering of Aviation in New Zealand",
    .author = "Leo White",
    .owner = "Peter Dann King",
    .address = "76 Prospect Terrace, Mt Eden, Auckland",
    .postal_code = "S.2",
    .reserve_nzd = 150,
    .latitude_fixed = -368884,
    .longitude_fixed = 1747530,
    .incident_date = "1942-03-13",
    .flight_path_convent = "Home of the Good Shepherd (Waikowhai Rd, Mt Roskill)",
    .flight_path_cemetery = "Hillsborough Cemetery",
    .flight_path_exit = "Manukau Heads",
    .color_profile_hex = 0x0011E6D4
};
SRC_EOF

echo "[✔] archive.c written successfully."

# 2. Automatically inject archive.o into your Makefile build targets
if [ -f "z_os/Makefile" ]; then
    echo "=== UPDATING MAKEFILE BUILD STRUCTURE ==="
    
    # Backup original Makefile
    cp z_os/Makefile z_os/Makefile.bak
    
    # Inject archive.o directly into the dependency tree right after main.o
    sed -i 's/main.o/main.o src\/archive.o/g' z_os/Makefile 2>/dev/null || \
    sed -i 's/src\/main.o/src\/main.o src\/archive.o/g' z_os/Makefile
    
    echo "[✔] Makefile successfully updated with archive compilation target."
else
    echo "[!] Makefile missing from z_os directory root."
fi

