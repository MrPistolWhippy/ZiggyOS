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
