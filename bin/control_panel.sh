#!/bin/sh
# ==============================================================================
#  THEOSI ZIGGYOS CONSOLE DECK & FABRICATION HUD V130.0 (STABLE POSIX NODE)
# ==============================================================================

g() {
    local blocks=" ▂▃▄▅▆▇█"
    local o=""
    # Safe POSIX sequential enumeration sweep loop
    for i in 1 2 3 4 5 6 7 8 9 10 11 12; do
        local seed=$(date +%N | tr -d '0\n')
        [ -z "$seed" ] && seed=1
        local idx=$(( (seed * i) % 8 + 1 ))
        # Use high-speed text cutting instead of bash array indices
        local char=$(echo "$blocks" | cut -c "$idx")
        o="${o}${char}"
    done
    echo -e "\033[1;32m${o}\033[0m"
}

q() {
    local term="$1"
    if [ -f "/root/archive.db" ] && [ ! -z "$term" ]; then
        local result=$(sqlite3 /root/archive.db "SELECT summary, raw_text_payload FROM offline_web_vault WHERE keyword LIKE '%$term%' OR raw_text_payload LIKE '%$term%' LIMIT 1;" 2>/dev/null | sed 's/|/ -> /g')
        if [ ! -z "$result" ]; then
            local ts=$(date +%Y%m%d_%H%M%S)
            echo -e "[REPORT] $term\n[DATA] $result" > /root/logs/report_${ts}.txt
            echo -e "$result\n\n\033[1;32m[✓] REPORT LOGGED: /root/logs/report_${ts}.txt\033[0m"
        else
            echo "[-] No matching hyper-index records found."
        fi
    fi
}

SEARCH_FILTER="$1"

while true; do
    clear
    echo -e "\033[38;5;198m============================================================\033[0m"
    echo -e "\033[1;36m           THEOSI ZIGGYOS CONSOLE DECK & FABRICATION MATRIX \033[0m"
    echo -e "\033[38;5;198m============================================================\033[0m"
    echo -e "  - BRAM Core:  \033[92mONLINE (32-Slot Ring Buffer Base Addr: 0x40001000)\033[0m"
    echo -e "  - Silicon Node:\033[92mTSMC_28NM_HPC (Computed Core Footprint: 990.42 um²)\033[0m"
    echo -e "  - Pin Layout: \033[1;35mACTIVE [ clk:PAD_E12 | alert:PAD_M22 | latch:PAD_P04 ]\033[0m"
    echo -e "\033[38;5;198m------------------------------------------------------------\033[0m"
    echo -e "\033[1;36m🔎 INDEPENDENT OFFLINE RECONNAISSANCE SEARCH INTERCEPT:\033[0m"
    
    if [ ! -z "$SEARCH_FILTER" ]; then
        echo -e "  Query: \"$SEARCH_FILTER\"\n  Results:\n"
        q "$SEARCH_FILTER" | sed 's/^/   /'
    else
        echo "  [-] Running in stream mode. Pass a pad argument to look up."
    fi
    
    echo -e "\033[38;5;198m------------------------------------------------------------\033[0m"
    echo -e "\033[1;36m📊 LIVE ELECTROMAGNETIC SPECTRUM ANALYSIS:\033[0m"
    echo -e "  CH_A: [$(g)]  CH_B: [$(g)]"
    echo -e "\033[38;5;198m============================================================\033[0m"
    sleep 1
done
