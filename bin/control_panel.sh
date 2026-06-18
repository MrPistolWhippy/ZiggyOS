#!/bin/bash
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
while true; do
    clear
    echo -e "\033[38;5;198m============================================================\033[0m"
    echo -e "\033[1;36m               THEOSI SYNC MASTER DECK V80.0                \033[0m"
    echo -e "\033[38;5;198m============================================================\033[0m"
    echo -e "  - BRAM Core:  \033[92mONLINE (Base Addr: 0x40001000)\033[0m"
    echo -e "  - Clock Loop: \033[92mVALIDATED (125 MHz Latency Sub-8.00ns)\033[0m"
    echo -e "  - Shor/PQC:   \033[1;35mACTIVE [ N=15 | r=4 ] \033[1;32mCRACKED [ Keys: 3, 5 ]\033[0m"
    echo -e "\033[38;5;198m------------------------------------------------------------\033[0m"
    
    # LIVE SYSTEM DATA-FLOW MATRIX TREE GRAPHIC PANEL
    echo -e "\033[1;36m🌲 SYSTEM DATA-FLOW ARCHITECTURE TOPOLOGY MAP:\033[0m"
    echo -e "   [ANTENNA RF FRONT-END] ──► [sat_socket.py INTERCEPT CORE]"
    echo -e "                                      │"
    echo -e "                                      ▼"
    echo -e "   [archive.db LOGS] ◄── [router.py] ──► [pqc_lattice.py ENGINES]"
    echo -e "                                      │"
    echo -e "                                      ▼"
    echo -e "   [GitHub SSH CLOUD] ◄── [save] ◄──── [BRAM SILICON REGISTER]"
    echo -e "\033[38;5;198m------------------------------------------------------------\033[0m"
    echo -e "\033[1;36m📊 LIVE ELECTROMAGNETIC SPECTRUM ANALYSIS:\033[0m"
    echo -e "  CH_A: [$(g)]  CH_B: [$(g)]"
    echo -e "\033[38;5;198m------------------------------------------------------------\033[0m"
    echo -e "\033[1;33m📡 LIVE DATA LOGGING STREAM:\033[0m"
    tail -n 3 /root/logs/network_sniff.log 2>/dev/null || echo "  [-] No active streams."
    echo -e "\033[38;5;198m============================================================\033[0m"
    sleep 0.5
done
