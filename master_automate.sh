#!/usr/bin/env bash
set -uo pipefail
G='\033[0;32m'; C='\033[0;36m'; Y='\033[0;33m'; R='\033[0;31m'; NC='\033[0m'

show_menu() {
    clear
    echo -e "${C}=== THEOS1 ZIGGYOS OMNI-DECK V180.0 ===${NC}"
    echo -e " [1] RUN INTEGRITY RECONSWEEP (Check Ledger Hashing)"
    echo -e " [2] VIEW ACTIVE TELEMETRY   (Safe ps Core Audit)"
    echo -e " [3] INGEST RAW DATA STREAM  (Launch Byte Parser)"
    echo -e " [4] COMPILE RISC-V CORE     (Assemble Machine Code)"
    echo -e " [5] TRIGGER AUTO-PILOT RUN  (Consolidate & Auto-Sync)"
    echo -e " [6] EXIT CONSOLE MAIN ENGINE"
    echo -n "ENTER ACTION TARGET VECTOR [1-6]: "
}

run_1() { echo -e "\n${C}[*] RUNNING CRYPTOGRAPHIC AUDIT...${NC}"; /root/mesh_omni_core.sh --check; read -n 1 -p "..."; }
run_2() { echo -e "\n${C}[*] FETCHING ACTIVE PROCESS PIDs...${NC}"; ps w | grep -E "PID|mesh_omni|master|auto_pilot" | grep -v grep; read -n 1 -p "..."; }
run_3() { echo -e "\n${C}[*] PARSING INCOMING PACKET STREAM...${NC}"; python3 /root/swarm_parser.py; read -n 1 -p "..."; }
run_4() { echo -e "\n${C}[*] RE-COMPILING RV32I HARDWARE OBJECT...${NC}"; riscv-none-elf-gcc -c /root/riscv_watchdog.s -o /root/watchdog.o && echo "SUCCESS" || echo "Cached"; read -n 1 -p "..."; }
run_5() { echo -e "\n${Y}[*] INVOKING FULL SYSTEM AUTO-PILOT CYCLE...${NC}"; /root/auto_pilot.sh; read -n 1 -p "..."; }

while true; do
    show_menu; read -r choice
    case "$choice" in
        1) run_1 ;; 2) run_2 ;; 3) run_3 ;; 4) run_4 ;; 5) run_5 ;;
        6) clear; echo "Console suspended."; exit 0 ;;
        *) echo -e "${R}Invalid choice.${NC}"; sleep 1 ;;
    esac
done
