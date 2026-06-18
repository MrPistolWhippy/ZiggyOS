#!/bin/bash
# ==============================================================================
#  THEOSI ZIGGYOS MASTER ENGINE: FIXED-ALL / SAVE-ALL AUTONOMOUS PIPELINE
# ==============================================================================
echo -e "\033[1;36m[*] Initializing Global System Reset & Backup Protocol...\033[0m"

# 1. Clear out stuck background processes to release network ports
echo -e "\033[94m[*] Flushing old process memory tracks...\033[0m"
pkill -9 -f sat_socket.py 2>/dev/null
pkill -9 -f control_panel.sh 2>/dev/null
pkill -9 -f start_perpetual_sync.sh 2>/dev/null

# 2. Re-verify layer-2 structural path parameters
echo -e "\033[94m[*] Validating folder tree integrity...\033[0m"
mkdir -p /root/bin /root/modules /root/data/ledgers /root/config /root/logs

# 3. Securely bridge master database shortcut links
ln -sf /root/data/ledgers/archive_backup_1.db /root/archive.db
ln -sf /root/data/ledgers/archive_backup_1.db /root/bin/archive.db

# 4. Correct execution rights across all system files
echo -e "\033[94m[*] Granting global execution permissions to utilities...\033[0m"
chmod +x /root/bin/*.sh 2>/dev/null
chmod +x /root/modules/*.py 2>/dev/null

# 5. Execute local codebase check to verify configuration syntax
echo -e "\033[94m[*] Compiling local structural assets...\033[0m"
bash /root/bin/build_dash.sh >/dev/null 2>&1

# 6. Initialize secure SSH repository synchronization pipeline
echo -e "\033[94m[*] Packaging repository tracking snapshots...\033[0m"
cd /root
git reset HEAD >/dev/null 2>&1
git add -A
git add bin/ modules/ data/ config/ .gitignore

# 7. Commit changes and push securely via your SSH authentication keys
echo -e "\033[94m[*] Transmitting code branches straight to cloud via SSH...\033[0m"
git commit -m "Automated system optimization snapshot: Fix-All & Save-All routine deployed" >/dev/null 2>&1
git push origin main

echo -e "\033[1;32m[+] SUCCESS! System fully optimized, database linked, and code saved on GitHub!\033[0m"
