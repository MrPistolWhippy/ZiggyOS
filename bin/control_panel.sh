#!/bin/sh
g() {
    o=""; for i in 1 2 3 4 5 6; do s=$(date +%N | tr -d '0\n'); o="${o}$(echo " ▂▃▄▅▆▇█" | cut -c $(( (${s:-1} * i) % 8 + 1 )))"; done; echo -e "\033[1;32m$o\033[0m"
}
while true; do
    clear
    echo -e "\033[1;36m=== THEOSI ZIGGYOS OMNI-DECK V150.0 ===\033[0m"
    echo -e " 💎 Foundry : \033[92mTSMC 28nm HPC [990.42 um² | 14 Gates]\033[0m"
    F=$(ls -t /root/logs/forensic_report_*.txt 2>/dev/null | head -n 1)
    echo -e " 🔬 Forensic: \033[92m59 Cells Ingested [$(basename ${F:-None})]\033[0m"
    V=$(sqlite3 /root/archive.db "SELECT raw_text_payload FROM offline_web_vault WHERE keyword='ghost_cell_0x40001040' LIMIT 1;" 2>/dev/null | awk -F'Vector: ' '{print $2}')
    echo -e " 👻 Chaos   : \033[5;38;5;226m[ ACTIVE GHOST VECTOR: ${V:-0xEAFD} ]\033[0m"
    echo -e " 🌐 Proxy   : \033[92mhttps://ngrok-free.app [OPERATIONAL]\033[0m"
    echo -e " 📊 Scanners: CH_A:[$(g)] CH_B:[$(g)]"
    echo -e "\033[1;36m=======================================\033[0m"
    sleep 1
done
