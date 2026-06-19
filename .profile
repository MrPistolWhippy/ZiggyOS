alias deck='/root/bin/deck_router.sh'
alias ship='/root/bin/git_push_string.sh'
alias save='/root/bin/fix_and_save_all.sh'

# Custom ZiggyOS Omni-Deck Prompt Scheme Configuration

# Automated ZiggyOS SSH Server Lifecycle Provisioning
if ! pgrep -x "sshd" > /dev/null; then
    /usr/sbin/sshd -p 2222 > /dev/null 2>&1
    echo "[+] Remote SSH Gateway Engine initialized on port 2222."
fi

# Automated ZiggyOS Node.js API Server Lifecycle Provisioning
if ! pgrep -f "server.js" > /dev/null; then
    node /root/workspace/bin/server.js > /dev/null 2>&1 &
    echo "[+] Advanced Workspace API streaming framework active on port 3000."
fi
alias boot='./workspace/bin/boot_cockpit.sh'
alias probe='/root/workspace/bin/run_kernel.sh'

# Global Absolute Path Architecture Configuration
export PATH="/bin:/usr/bin:/sbin:/usr/sbin:/root/workspace/bin"
alias sweep='/root/workspace/bin/sweep_deck.sh'
export PS1="\[\033[1;31m\][SCOUTER]\[\033[1;31m\][KI-POWER-V9000] # \[\033[0m\]"

# ---------------------------------------------------------------------
# DECENTRALIZED MESH ENVIRONMENT: AUTO-STARTUP SECTOR
# ---------------------------------------------------------------------
# Check if the omni-core monitoring engine is currently running in memory
if ! ps aux | grep -v grep | grep -q "mesh_omni_core.sh --daemon"; then
    echo -e "\033[0;36m[*] OMNI-CORE: Background daemon dead or inactive. Initializing watchdog...\033[0m"
    nohup /root/mesh_omni_core.sh --daemon > /root/monitor.log 2>&1 &
    echo -e "\033[0;32m[+] Watchdog attached to background thread safely.\033[0m"
else
    echo -e "\033[0;32m[*] OMNI-CORE: Background monitoring daemon is already active and verified.\033[0m"
fi

# Run an instant diagnostic parity sweep right on login to display active status
/root/mesh_omni_core.sh --check
echo "---------------------------------------------------------------------"

# Custom Global Dashboard Diagnostic Shortcut
dash() {
    echo -e "\033[0;36m=== NODE p20 SYSTEM STATUS METRICS ===\033[0m"
    echo -e "1. Active Daemons running in background thread:"
    ps w | grep "mesh_omni" | grep -v grep || echo "   [!] Warning: Watchdog daemon inactive."
    echo -e "\n2. Primary Ledger Integrity State:"
    if [ -f "/root/mesh_topology_ledger.db" ]; then
        sha256sum /root/mesh_topology_ledger.db
    else
        echo "   [!] Error: Primary ledger missing."
    fi
    echo -e "\n3. Replicated Mirror Partition State:"
    if [ -f "/root/iSh files/mesh_node_backup/mesh_topology_ledger.bak" ]; then
        ls -lh /root/iSh files/mesh_node_backup/mesh_topology_ledger.bak
    else
        echo "   [!] Warning: Backup replication mirror empty."
    fi
    echo -e "\033[0;36m======================================\033[0m"
}
dash() {
    echo -e "\033[0;36m=== NODE p20 SYSTEM STATUS METRICS ===\033[0m"
    echo -e "1. Active Daemons running in background thread:"
    ps w | grep "mesh_omni" | grep -v grep || echo "   [!] Warning: Watchdog daemon inactive."
    echo -e "\n2. Primary Ledger Integrity State:"
    if [ -f "/root/mesh_topology_ledger.db" ]; then
        sha256sum /root/mesh_topology_ledger.db
    else
        echo "   [!] Error: Primary ledger missing."
    fi
    echo -e "\n3. Replicated Mirror Partition State:"
    # Strict variable boundary masking to lock down whitespace parameters
    if [ -f "/root/iSh files/mesh_node_backup/mesh_topology_ledger.bak" ]; then
        ls -lh "/root/iSh files/mesh_node_backup/mesh_topology_ledger.bak"
    else
        echo "   [!] Warning: Backup replication mirror empty."
    fi
    echo -e "\033[0;36m======================================\033[0m"
}
alias essential='/root/master_automate.sh'
alias essential='/root/master_automate.sh'
dash() {
    echo -e "\033[0;36m=== NODE p20 SYSTEM STATUS METRICS ===\033[0m"
    echo -e "1. Active Daemons running in background thread:"
    ps w | grep "mesh_omni" | grep -v grep || echo "   [!] Warning: Watchdog daemon inactive."
    echo -e "\n2. Primary Ledger Integrity State:"
    [ -f "/root/mesh_topology_ledger.db" ] && sha256sum /root/mesh_topology_ledger.db || echo "   [!] Error: Primary ledger missing."
    echo -e "\n3. Replicated Mirror Partition State:"
    [ -f "/root/iSh files/mesh_node_backup/mesh_topology_ledger.bak" ] && ls -lh "/root/iSh files/mesh_node_backup/mesh_topology_ledger.bak" || echo "   [!] Warning: Backup mirror empty."
    echo -e "\n4. RISC-V Bare-Metal Object Compilation State:"
    [ -f "/root/watchdog.o" ] && ls -lh /root/watchdog.o || echo "   [!] Status: RISC-V core assembly not yet compiled."
    echo -e "\033[0;36m======================================\033[0m"
}
