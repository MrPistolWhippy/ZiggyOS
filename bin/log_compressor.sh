#!/bin/ash
# ==============================================================================
#   ZIGGY-OS SYSTEM AUTOMATED LOG COMPRESSION & MEMORY BUFFER PROTECTOR
# ==============================================================================
LOG_FILE="/root/sweep_output.log"
TARGET_IMG="/root/virtual_disk.img"
MAX_SIZE=1048576 # 1 Megabyte threshold limit in bytes

while true; do
    if [ -f "$LOG_FILE" ]; then
        SIZE=$(wc -c < "$LOG_FILE")
        
        # Check if the active stream has breached the 1MB ceiling threshold
        if [ "$SIZE" -gt "$MAX_SIZE" ]; then
            TIMESTAMP=$(date +%Y%m%d_%H%M%S)
            ARCHIVE="/root/sweep_archive_${TIMESTAMP}.tar.gz"
            
            echo -e "\n[!] LOG BREAK DETECTED: Compress pass initialized to safeguard RAM overhead..." >> "$LOG_FILE"
            
            # 1. Compress the current telemetry log file at maximum density
            tar -czf "$ARCHIVE" "$LOG_FILE" 2>/dev/null
            
            # 2. Mirror the archive securely down into your air-gapped FAT container sectors
            if [ -f "$TARGET_IMG" ]; then
                mcopy -o -i "$TARGET_IMG" "$ARCHIVE" ::/ 2>/dev/null
            fi
            
            # 3. Truncate the live log file cleanly to 100 entries to reclaim storage blocks
            tail -n 100 "$LOG_FILE" > "${LOG_FILE}.tmp"
            mv "${LOG_FILE}.tmp" "$LOG_FILE"
            rm -f "$ARCHIVE"
            
            echo -e "\033[1;91m[+] WORKSPACE CLEANUP: Log compressed, archived to FAT, and truncated.\033[0m"
        fi
    fi
    # Perform a system data check pass every 60 seconds
    sleep 60
done
