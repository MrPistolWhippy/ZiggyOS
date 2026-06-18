#!/bin/sh
echo -e "\033[1;35m[*] Injecting Polymorphic HUD Pipes & Variant Filter Macro...\033[0m"

# 1. Overwrite master HUD dashboard to integrate live mutating memory operation tracking
cat << 'HUD_V180' > /root/bin/control_panel.sh
#!/bin/sh
g() {
    local bars="##-##-##"
    local seed=$(date +%N | tr -d '0\n' | cut -c1-3)
    local idx=$(( (${seed:-1} % 4) * 2 + 1 ))
    echo -e "\033[1;32m$(echo "$bars" | cut -c ${idx}-$((idx+1)))\033[0m"
}
while true; do
    clear
    echo -e "\033[1;36m=== THEOSI ZIGGYOS OMNI-DECK V180.0 ===\033[0m"
    echo -e " 💎 Foundry : \033[92mTSMC 28nm HPC [990.42 um² | 12 Gates]\033[0m"
    F=$(ls -t /root/logs/forensic_report_*.txt 2>/dev/null | head -n 1)
    echo -e " 🔬 Forensic: \033[92m59 Cells Ingested [$(basename ${F:-None})]\033[0m"
    
    # NEW LIVE MUTATING MEMORY INTERCEPT HUD READOUT ROW
    M_OP=$(sqlite3 /root/archive.db "SELECT vector_payload FROM bit_packed_net WHERE mask_key='polymorphic_gate' ORDER BY bit_id DESC LIMIT 1;" 2>/dev/null | grep -o "Token: .*" | awk '{print $2}')
    echo -e " 🧬 Mutation: \033[5;38;5;198m[ MUTATING MEMORY BLOCK: ${M_OP:-0x0C1F} ]\033[0m"
    
    TOTAL_NODES=$(sqlite3 /root/archive.db "SELECT COUNT(*) FROM bit_packed_net;" 2>/dev/null)
    echo -e " ♒ Aquarius: \033[1;35mP2P REALITY UNIFIED [ Cached Nodes: ${TOTAL_NODES:-5} ]\033[0m"
    echo -e " 📊 Scanners: CH_A:[$(g)] CH_B:[$(g)]"
    echo -e "\033[1;36m=======================================\033[0m"
    sleep 1
done
HUD_V180
chmod +x /root/bin/control_panel.sh
cp /root/bin/control_panel.sh /bin/deck
chmod +x /bin/deck
echo "[✓] Control Deck pipe architecture updated."

# 2. Deploy the custom high-speed variant memory filter scanner macro tool
cat << 'VARIANT_SCAN' > /root/bin/findvariant
#!/bin/sh
TERM="$1"
DB="/root/archive.db"
echo -e "\033[38;5;198m============================================================\033[0m"
echo -e "\033[1;36m           ZIGGYOS POLYMORPHIC VARIANT SCANNER ENGINE       \033[0m"
echo -e "\033[38;5;198m============================================================\033[0m"
if [ -z "$TERM" ]; then echo "Usage: findvariant <token_or_payload_keyword>"; exit 0; fi
if [ -f "$DB" ]; then
    sqlite3 "$DB" "SELECT bit_id, vector_payload FROM bit_packed_net WHERE mask_key='polymorphic_gate' AND vector_payload LIKE '%$TERM%';" 2>/dev/null
else
    echo "[-] Memory-mapped matrix unlinked."
fi
echo -e "\033[38;5;198m============================================================\033[0m"
VARIANT_SCAN
chmod +x /root/bin/findvariant
ln -sf /root/bin/findvariant /bin/findvariant
echo "[✓] Variant memory scanner macro compiled to system paths."

echo -e "\033[1;32m[+] SUCCESS! Both polymorphic sub-modules active.\033[0m"
