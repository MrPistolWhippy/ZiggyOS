#!/bin/bash
# ==============================================================================
#  THEOSI DECK MASTER SYSTEM ROUTER - STABILITY WRAPPER V2
# ==============================================================================
echo -e "\033[94m[*] Initializing Layer-2 System Verification...\033[0m"

# A. Verify Database Bridge Core
if [ ! -L "/root/archive.db" ]; then
    echo -e "\033[91m[!] Missing master database symlink. Repairing links...\033[0m"
    ln -sf /root/data/ledgers/archive_backup_1.db /root/archive.db
fi

# B. Check for active background Daemons
if ! pgrep -f "sleep 40260" > /dev/null; then
    echo -e "\033[93m[!] Sync daemon inactive. Triggering Background Scheduler...\033[0m"
    cd /root/bin && ./start_perpetual_sync.sh > /root/logs/sync_daemon.log 2>&1 &
fi

# C. Keep Promiscuous User-Space Sniffer running in background
if ! pgrep -f "sat_socket.py" > /dev/null; then
    echo -e "\033[96m[+] Initializing Background Network Telemetry Sniffer...\033[0m"
    python3 /root/modules/sat_socket.py > /dev/null 2>&1 &
fi

# D. Launch Control Core UI Environment
echo -e "\033[92m[+] Verification clean. Booting Core Telemetry Matrix...\033[0m"
sleep 1
cd /root/bin && ./control_panel.sh
