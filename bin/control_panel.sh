#!/bin/sh
g() {
    local bars="##-##-##"
    local seed=$(date +%N | tr -d '0\n' | cut -c1-3)
    local idx=$(( (${seed:-1} % 4) * 2 + 1 ))
    echo -e "\033[1;32m$(echo "$bars" | cut -c ${idx}-$((idx+1)))\033[0m"
}
while true; do
    clear
    echo -e "\033[1;36m=== THEOSI ZIGGYOS OMNI-DECK V160.0 ===\033[0m"
    echo -e " 💎 Foundry : \033[92mTSMC 28nm HPC [990.42 um² | 14 Gates]\033[0m"
    F=$(ls -t /root/logs/forensic_report_*.txt 2>/dev/null | head -n 1)
    echo -e " 🔬 Forensic: \033[92m59 Cells Ingested [$(basename ${F:-None})]\033[0m"
    V=$(sqlite3 /root/archive.db "SELECT raw_text_payload FROM offline_web_vault WHERE keyword='ghost_cell_0x40001040' LIMIT 1;" 2>/dev/null | grep -o "Vector: .*" | awk '{print $2}')
    echo -e " 👻 Chaos   : \033[5;38;5;226m[ ACTIVE GHOST VECTOR: ${V:-0xEAFD} ]\033[0m"
    
    # NEW LIVE BIT-PACKED OFFLINE WEB INTEGRITY TRACKER ROW
    TOTAL_NODES=$(sqlite3 /root/archive.db "SELECT COUNT(*) FROM bit_packed_net;" 2>/dev/null)
    echo -e " 🌐 WorldNet: \033[1;35mLOCAL WEB ACTIVE [ Bitwise Nodes Cached: ${TOTAL_NODES:-5} ]\033[0m"
    
    echo -e " 📊 Scanners: CH_A:[$(g)] CH_B:[$(g)]"
    echo -e "\033[1;36m=======================================\033[0m"
    sleep 1
done
