#!/bin/ash
LOG_FILE="/root/sweep_output.log"
MAX_SIZE=500000 # Max target storage footprint in bytes (~500KB)

if [ -f "$LOG_FILE" ]; then
    SIZE=$(wc -c < "$LOG_FILE")
    if [ "$SIZE" -gt "$MAX_SIZE" ]; then
        echo "[!] Storage buffer cap hit. Performing log truncation pass..."
        tail -n 100 "$LOG_FILE" > "${LOG_FILE}.tmp"
        mv "${LOG_FILE}.tmp" "$LOG_FILE"
        echo "[+] Log context cleared. Last 100 historical entries retained."
    fi
fi
