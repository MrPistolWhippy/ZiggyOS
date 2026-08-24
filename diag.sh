#!/bin/bash
echo "⚡ DECENTRALIZED OPERATIONS CENTRE [MINT-CORE v19.0] ⚡"
while true; do
    read -p "MrPistolWhippy@Flux-Core:~# " cmd
    case "$cmd" in
        "exit"|"quit"|"root") echo "Disengaging panel. Returning to native shell."; break ;;
        "help") echo "Available Commands: help, status, flux-scan, decentral-test, init-rtos, root, exit" ;;
        "status") echo "--- Node Matrix Status ---"; echo "Internal Core IP: $(hostname -i)"; echo "Uptime Signature: $(uptime | sed 's/.*up //; s/,  load.*//')"; echo "Telemetry Cluster: LOADED (Waitematā Sector)";; 
        "flux-scan") echo "--- Executing Pure-Bash Subnet Probe ---"; ip_base=$(hostname -i | cut -d. -f1-3); [ -z "$ip_base" ] && ip_base="127.0.0"; echo "Tracing active pathways on ${ip_base}.0/24..."; for i in $(seq 1 15); do ping -c 1 -W 1 ${ip_base}.${i} >/dev/null 2>&1 && echo " -> Active Flux Node Detected: ${ip_base}.${i}" & done; wait; echo "Subnet matrix trace complete." ;;
        "decentral-test") echo "--- Auditing Bidirectional PQ-Tunnel ---"; if [ -f "pq_tunnel_bidirectional.py" ]; then python3 -m py_compile pq_tunnel_bidirectional.py 2>/dev/null && echo " -> pq_tunnel_bidirectional.py: Syntax Verified." || echo " -> Warning: Cryptographic syntax anomalies detected."; else echo " -> Error: Missing local pq_tunnel architecture files."; fi ;;
        "init-rtos") echo "--- Bootstrapping Ziggy RTOS Infrastructure Simulation ---"; if [ -f "launch_ziggy_rtos.sh" ]; then chmod +x launch_ziggy_rtos.sh rtos_bridge.sh 2>/dev/null; ./launch_ziggy_rtos.sh; else echo " -> Error: launch_ziggy_rtos.sh not found in this working directory."; fi ;;
        "") continue ;;
        *) clean_cmd=$(echo "$cmd" | cut -d" " -f1); if [ -n "$clean_cmd" ] && command -v "$clean_cmd" >/dev/null 2>&1; then eval "$cmd"; else echo "Command not found in active namespace."; fi ;;
    esac
done
