#!/bin/sh
# Custom ANSI Escape Colors for an Electric Teal Screen Layout
TEAL='\033[38;5;38m'
BRIGHT_TEAL='\033[38;5;44m'
CYAN='\033[0;36m'
RED='\033[0;31m'
RESET='\033[0m'

clear
echo -e "${BRIGHT_TEAL}=========================================================${RESET}"
echo -e "${TEAL}[PRIDE PROTOCOL] ZIGGY-OS // SUPER SAIYAN ENGAGED${RESET}"
echo -e "${BRIGHT_TEAL}=========================================================${RESET}"
echo -e "${CYAN}[*] Initializing free-standing satellite array...${RESET}"
echo -e "${CYAN}[*] Frequency Sync: 144.777 MHz locked.${RESET}"
echo -e "${CYAN}[*] Routing Matrix Coordinates: 999.999.999.432${RESET}"
echo -e "${BRIGHT_TEAL}---------------------------------------------------------${RESET}"

# Verify if the database is accessible
if [ -f "/root/archive.db" ]; then
    echo -e "${BRIGHT_TEAL}[+] archive.db detected. Mapping topology targets...${RESET}"
else
    echo -e "${RED}[!] archive.db missing. Using baseline fallback loops.${RESET}"
fi

echo -e "${BRIGHT_TEAL}[+] BOOM BAM! Launching network transceiver loops now...${RESET}\n"

# Execute the core Python transceiver script directly
python3 /root/matrix_transceiver.py
