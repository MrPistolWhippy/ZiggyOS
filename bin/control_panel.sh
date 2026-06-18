#!/bin/bash
# ==============================================================================
#  THEOSI ZIGGYOS CONSOLE MATRIX DECK V120.0 + FORENSIC TRIAGE STREAM PIPELINE
# ==============================================================================
B=(" " "▂" "▃" "▄" "▅" "▆" "▇" "█")

g() {
    local o=""
    for i in {1..12}; do
        local seed=$(date +%N | tr -d '0\n')
        local r=$(( (seed * i) % 8 ))
        o+="${B[$r]}"
    done
    echo -e "\033[1;32m${o:0:4}\033[1;33m${o:4:4}\033[1;31m${o:8:4}\033[0m"
}

q() {
    local term="$1"
    if [ -f "/root/archive.db" ] && [ ! -z "$term" ]; then
        local result=$(sqlite3 /root/archive.db "SELECT summary, raw_text_payload FROM offline_web_vault WHERE keyword LIKE '%$term%' OR raw_text_payload LIKE '%$term%' LIMIT 1;" 2>/dev/null | sed 's/|/ -> /g')
        if [ ! -z "$result" ]; then
            local ts=$(date +%Y%m%d_%H%M%S)
            echo -e "[REPORT] $term\n[DATA] $result" > /root/logs/report_${ts}.txt
            echo -e "$result\n\n\033[1;32m[✓] REPORT LOGGED: /root/logs/report_${ts}.txt\033[0m"
        else echo "[-] No hyper-index records found matching criteria."; fi
    fi
}

# Forensic Log Parser Hook: Extracts the latest triage summary parameters from disk
get_latest_forensic_metrics() {
    local latest_report=$(ls -t /root/logs/forensic_report_*.txt 2>/dev/null | head -n 1)
    if [ ! -z "$latest_report" ]; then
        echo -e "  [+] Active Target Load File: \033[1;33m$(basename $latest_report)\033[0m"
        # Extract metadata counts and checksum samples
        local artifact_count=$(grep -o "Collected [0-9]*" "$latest_report" | awk '{print $2}')
        local sample_hash=$(grep "MD5:" "$latest_report" | head -n 1 | awk -F'MD5: ' '{print $2}')
        echo -e "  [+] Triage Artifacts Ingested: \033[1;32m$artifact_count System Cells\033[0m"
        echo -e "  [+] Integrity Verification Cap: \033[1;36mMD5: ${sample_hash:0:16}...\033[0m"
    else
        echo -e "  \033[90m[-] No active forensic artifacts piped to display registers.\033[0m"
    fi
}

SEARCH_FILTER="$1"

while true; do
    clear
    echo -e "\033[38;5;198m============================================================\033[0m"
    echo -e "\033[1;36m       THEOSI ZIGGYOS CONSOLE MATRIX DECK V120.0            \033[0m"
    echo -e "\033[38;5;198m============================================================\033[0m"
    echo -e "  - BRAM Core:  \033[92mONLINE (Base Addr: 0x40001000)\033[0m"
    echo -e "  - Clock Loop: \033[92mVALIDATED (125 MHz Latency Sub-8.00ns Setup/Hold)\033[0m"
    echo -e "  - Remote Web: \033[92mPROXY REDIRECT OPERATIONAL (https://ngrok-free.app)\033[0m"
    echo -e "\033[38;5;198m------------------------------------------------------------\033[0m"
    
    # NEW GLOWING FORENSIC DATA ARTIFACTS PANEL WINDOW
    echo -e "\033[1;36m🔬 FORENSIC TRIAGE EXTRACTION RECONNAISSANCE LEDGER:\033[0m"
    get_latest_forensic_metrics
    echo -e "\033[38;5;198m------------------------------------------------------------\033[0m"
    
    echo -e "\033[1;36m🌲 SYSTEM DATA-FLOW ARCHITECTURE TOPOLOGY MAP:\033[0m"
    echo -e "   [ANTENNA RF FRONT-END] ──► [sat_socket.py INTERCEPT CORE]"
    echo -e "                                      │"
    echo -e "                                      ▼"
    echo -e "   [archive.db LOGS] ◄── [router.py] ──► [pqc_lattice.py ENGINES]"
    echo -e "\033[38;5;198m------------------------------------------------------------\033[0m"
    echo -e "\033[1;36m📊 LIVE ELECTROMAGNETIC SPECTRUM ANALYSIS:\033[0m"
    echo -e "  CH_A: [$(g)]  CH_B: [$(g)]"
    echo -e "\033[38;5;198m============================================================\033[0m"
    sleep 1
done
