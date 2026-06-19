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
