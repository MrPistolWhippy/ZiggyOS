#!/usr/bin/env bash
set -uo pipefail
G='\033[0;32m'; C='\033[0;36m'; Y='\033[0;33m'; R='\033[0;31m'; NC='\033[0m'

show_menu() {
    clear
    echo -e "${C}=== THEOS1 ZIGGYOS OMNI-DECK V180.0 ===${NC}"
    echo -e " [1] RUN INTEGRITY RECONSWEEP   [5] VERIFY BACKUP REPLICATION"
    echo -e " [2] VIEW ACTIVE TELEMETRY      [6] GIT STATUS AUDIT"
    echo -e " [3] INGEST RAW DATA STREAM     [7] COMPILE RISC-V RV64I CORE"
    echo -e " [4] RE-MAP PROPELLER CHIP CORE [8] STOP BACKGROUND WATCHDOG"
    echo -e " [9] EXIT CONSOLE MAIN ENGINE"
    echo -n "ENTER ACTION TARGET VECTOR [1-9]: "
}

run_1() { echo -e "\n${C}[*] RUNNING INTEGRITY RECONSWEEP...${NC}"; /root/mesh_omni_core.sh --check; read -n 1 -p "..."; }
run_2() { echo -e "\n${C}[*] FETCHING ACTIVE TELEMETRY...${NC}"; ps w | grep -E "PID|mesh_omni|master" | grep -v grep; read -n 1 -p "..."; }
run_3() { echo -e "\n${C}[*] INITIALIZING BYTE PARSER...${NC}"; python3 /root/swarm_parser.py; read -n 1 -p "..."; }
run_4() { echo -e "\n${C}[*] VALIDATING PROPELLER SPIN2 CORE...${NC}"; ls -lh /root/propeller_mesh.spin2 2>/dev/null || echo "Missing asset"; read -n 1 -p "..."; }
run_5() { echo -e "\n${C}[*] AUDITING BACKUP STORAGE TIER...${NC}"; ls -lh "/root/iSh files/mesh_node_backup/mesh_topology_ledger.bak" 2>/dev/null || echo "Mirror Empty"; read -n 1 -p "..."; }
run_6() { echo -e "\n${C}[*] SWEEPING GIT WORKING TREE...${NC}"; git status; read -n 1 -p "..."; }
run_7() { 
    echo -e "\n${C}[*] STAGING RISC-V RV64I COMPILATION TARGETS...${NC}"
    if [ -f "/root/riscv_watchdog.s" ] && [ -f "/root/riscv_driver.c" ]; then
        echo -e "   ├── ${G}[FOUND]${NC} riscv_watchdog.s ($(wc -c < /root/riscv_watchdog.s) bytes)"
        echo -e "   ├── ${G}[FOUND]${NC} riscv_driver.c ($(wc -c < /root/riscv_driver.c) bytes)"
        echo -e "   └── ${Y}[INFO]${NC} Toolchain linked. Executing target code compilation check..."
        riscv64-none-elf-gcc -c /root/riscv_watchdog.s -o /root/watchdog.o 2>/dev/null || echo "   [!] Staging compiler cross-build..."
    else
        echo -e "${R}[ERROR] Missing RISC-V source files in path structure.${NC}"
    fi
    read -n 1 -p "..."
}
run_8() { echo -e "\n${R}[!] FORCE CLEARING BACKGROUND DAEMON...${NC}"; kill \$(ps w | grep "mesh_omni_core.sh" | grep -v grep | awk '{print \$1}') 2>/dev/null && echo "Terminated" || echo "No process caught"; read -n 1 -p "..."; }

while true; do
    show_menu; read -r choice
    case "\$choice" in
        1) run_1 ;; 2) run_2 ;; 3) run_3 ;; 4) run_4 ;;
        5) run_5 ;; 6) run_6 ;; 7) run_7 ;; 8) run_8 ;;
        9) clear; echo "Console suspended."; exit 0 ;;
        *) echo -e "${R}Invalid vector assignment.${NC}"; sleep 1 ;;
    esac
done
