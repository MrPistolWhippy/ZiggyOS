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

q() {
    local term="$1"
    if [ -f "/root/archive.db" ] && [ ! -z "$term" ]; then
        # Extracted the direct variable assignment with flat shell parsing rules
        local result=$(sqlite3 /root/archive.db "SELECT summary, raw_text_payload FROM offline_web_vault WHERE keyword LIKE '%$term%' OR raw_text_payload LIKE '%$term%' LIMIT 1;" 2>/dev/null | sed 's/|/ -> /g')
        
        if [ ! -z "$result" ]; then
            local ts=$(date +%Y%m%d_%H%M%S)
            echo -e "[REPORT] $term\n[DATA] $result" > /root/logs/report_${ts}.txt
            echo -e "$result\n\n\033[1;32m[✓] REPORT LOGGED: /root/logs/report_${ts}.txt\033[0m"
        else
            echo "[-] No hyper-index records found matching criteria."
        fi
    fi
}

SEARCH_FILTER="$1"

while true; do
    clear
    echo -e "\033[38;5;198m============================================================\033[0m"
    echo -e "\033[1;36m           THEOSI ZIGGYOS CONSOLE DECK & HYPER-INDEX NET    \033[0m"
    echo -e "\033[38;5;198m============================================================\033[0m"
    echo -e "  - Net Status: \033[1;35mHYPER-INDEX LOCAL WEB ACTIVE (Offline Node)\033[0m"
    echo -e "\033[38;5;198m------------------------------------------------------------\033[0m"
    echo -e "\033[1;36m🔎 INDEPENDENT OFFLINE RECONNAISSANCE SEARCH INTERCEPT:\033[0m"
    
    if [ ! -z "$SEARCH_FILTER" ]; then
        echo -e "  Query: \"$SEARCH_FILTER\"\n  Results:\n"
        q "$SEARCH_FILTER" | sed 's/^/   /'
    else
        echo "  [-] Running in stream mode. Pass an argument to extract."
    fi
    
    echo -e "\033[38;5;198m------------------------------------------------------------\033[0m"
    echo -e "\033[1;36m📊 LIVE ELECTROMAGNETIC SPECTRUM ANALYSIS:\033[0m"
    echo -e "  CH_A: [$(g)]  CH_B: [$(g)]"
    echo -e "\033[38;5;198m============================================================\033[0m"
    sleep 1.5
done
