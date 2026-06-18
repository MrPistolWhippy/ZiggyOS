#!/bin/bash
LOG="/root/transmission_diary.log"
echo -e "\n\033[38;5;220m[+] RETRIEVING AUTOMATED BACKGROUND AUTOMATION LOGS...\033[0m"
if [ -f "$LOG" ]; then
    # Scan for the last 3 cron audit signatures written to disk
    grep -E "ZIGGY-OS|AUDIT" "$LOG" | tail -n 3
else
    echo -e "  [-] No active automated log trail found on disk yet."
fi
