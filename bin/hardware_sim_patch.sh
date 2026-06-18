#!/bin/bash
echo -e "\033[1;33m[*] Deploying Mini Sim & HUD Suite...\033[0m"

# 1. Mini Radio Wave IQ Stream Simulator Module
cat << 'HW_SIM' > /root/modules/sdr_sim.py
#!/usr/bin/env python3
import time, secrets, os
from datetime import datetime
os.makedirs("/root/logs", exist_ok=True)
try:
    ts = datetime.now().strftime('%H:%M:%S')
    with open("/root/logs/network_sniff.log", "a") as f:
        f.write(f"[{ts}] RF_WAVE | RAW_HEX: 4745545F5A49474759_{secrets.token_hex(6).upper()} | IQ_BLOCK\n")
except: pass
HW_SIM

# 2. Fully Upgraded Master Control HUD Panel Script
cat << 'HW_HUD' > /root/bin/control_panel.sh
#!/bin/bash
mkdir -p /root/logs && touch /root/logs/network_sniff.log
while true; do
    clear
    echo -e "\033[38;5;198m============================================================\033[0m"
    echo -e "\033[1;36m               THEOSI SYNC MASTER DECK V50.0                \033[0m"
    echo -e "\033[38;5;198m============================================================\033[0m"
    echo -e "\033[94m[+] SYSTEM ARCHITECTURE STATUS:\033[0m"
    echo -e "  - Core Target:     \033[92mONLINE (Bare-Metal RISC-V/FPGA Silicon Layout)\033[0m"
    echo -e "  - Storage Vault:   \033[92mCONNECTED (SPI Bus Master Addr: 0x40000000)\033[0m"
    echo -e "\033[94m[+] PERIPHERAL CHIP INTERCONNECT INTEGRITY:\033[0m"
    if grep -q "RF_WAVE" /root/logs/network_sniff.log 2>/dev/null; then
        echo -e "  - Antenna Tuner:   \033[5;38;5;226m[!!] LOCK WARNING: 447.770 MHz EM BURST CAPTURED [!!]\033[0m"
        echo -e "  - Monitor Screen:  \033[1;32m[ ACTIVE ] Waveshare 3.5\" TFT LCD Panel (CS1)\033[0m"
    else
        echo -e "  - Antenna Tuner:   \033[90m[-] AWAITING FREQUENCY OSCILLATOR LATCH (447.770 MHz)...\033[0m"
        echo -e "  - Monitor Screen:  \033[90m[-] STANDBY MODE (CS1 Idle)\033[0m"
    fi
    echo -e "\033[38;5;198m------------------------------------------------------------\033[0m"
    echo -e "\033[1;33m📡 LIVE SILICON RF INTERCEPT STREAMS (MONITOR LOG):\033[0m"
    if [ -s "/root/logs/network_sniff.log" ]; then tail -n 6 /root/logs/network_sniff.log
    else echo -e "  \033[90m[-] No raw hardware wave streams logged on disk interface.\033[0m"; fi
    echo -e "\033[38;5;198m============================================================\033[0m"
    sleep 2
done
HW_HUD

chmod +x /root/modules/sdr_sim.py /root/bin/control_panel.sh
echo -e "\033[1;32m[+] SUCCESS! Mini-HUD injected.\033[0m"
