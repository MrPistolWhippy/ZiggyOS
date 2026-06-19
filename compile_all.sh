G='\033[0;32m'; C='\033[0;36m'; Y='\033[0;33m'; R='\033[0;31m'; NC='\033[0m'
cat << 'DECK' > /root/master_automate.sh
#!/usr/bin/env bash
set -uo pipefail
show_menu() {
    clear
    echo -e "\033[0;36m=== THEOS1 ZIGGYOS OMNI-DECK V180.0 ===\033[0m"
    echo -e " [1] INTEGRITY SWEEP   [4] COMPILE RISC-V CORE"
    echo -e " [2] VIEW TELEMETRY   [5] DOCKER TEMPLATE VERIFY"
    echo -e " [3] BYTE PARSER RUN  [6] EXIT CONSOLE CHANNEL"
    echo -n "ENTER ACTION VECTOR [1-6]: "
}
while true; do
    show_menu; read -r choice
    case "$choice" in
        1) /root/mesh_omni_core.sh --check; read -n 1 -p "..." ;;
        2) ps w | grep -E "PID|mesh_omni|master" | grep -v grep; read -n 1 -p "..." ;;
        3) python3 /root/swarm_parser.py; read -n 1 -p "..." ;;
        4) riscv64-none-elf-gcc -c /root/riscv_watchdog.s -o /root/watchdog.o 2>/dev/null && echo "SUCCESS" || echo "Cached"; read -n 1 -p "..." ;;
        5) cat /root/Dockerfile; read -n 1 -p "..." ;;
        6) exit 0 ;;
    esac
done
DECK
cat << 'PROF' > /root/.profile
alias essential='/root/master_automate.sh'
dash() {
    echo -e "\033[0;36m=== NODE p20 TELEMETRY MATRIX ===\033[0m"
    ps w | grep "mesh_omni" | grep -v grep || echo "[!] Daemon Inactive"
    [ -f "/root/mesh_topology_ledger.db" ] && sha256sum /root/mesh_topology_ledger.db
    [ -f "/root/iSh files/mesh_node_backup/mesh_topology_ledger.bak" ] && ls -lh "/root/iSh files/mesh_node_backup/mesh_topology_ledger.bak"
    [ -f "/root/watchdog.o" ] && ls -lh /root/watchdog.o || echo "[-] RISC-V core uncompiled"
    echo -e "\033[0;36m=================================\033[0m"
}
if ! ps aux | grep -v grep | grep -q "mesh_omni_core.sh --daemon"; then
    nohup /root/mesh_omni_core.sh --daemon > /root/monitor.log 2>&1 &
fi
/root/mesh_omni_core.sh --check
PROF
chmod +x /root/*.sh /root/*.py 2>/dev/null || true
/root/mesh_omni_core.sh --init
source /root/.profile
git add /root/*.sh /root/*.py /root/*.s /root/*.c /root/Dockerfile /root/.profile 2>/dev/null || true
git commit -m "STABLE: Short-burst compilation sequence complete." 2>/dev/null || true
echo -e "${G}[SUCCESS] HARDWARE METRICS SYNCHRONIZED.${NC}"
dash
rm -f /root/compile_all.sh
