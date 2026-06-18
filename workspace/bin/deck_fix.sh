#!/bin/sh
# 1. Force add /root/workspace/bin permanently back to global execution paths
export PATH="$PATH:/root/workspace/bin"

# 2. Fix the global command aliases for all active windows and panes
alias essential="/root/workspace/bin/master_automate.sh"
alias ./essential="/root/workspace/bin/master_automate.sh"
alias master_automate.sh="/root/workspace/bin/master_automate.sh"

# 3. Create a pristine, crash-free top alternative specifically for iSH sandbox
alias top="ps -ef"

echo "[+] ZiggyOS Workspace paths and system aliases completely repaired!"
