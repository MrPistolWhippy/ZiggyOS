#!/bin/sh
echo "[*] INITIALIZING SELF-EVOLVING AUTOMATION ENGINE..."
SAFE_CORE_1="intel_pack.enc"
SAFE_CORE_2="intel_hex.stream"
SAFE_ENGINE="auto_engine.sh"
echo "[*] SCANNING ENVIRONMENT FOR FRAGMENT LEAKS..."
FRAG_LIST=$(find . -maxdepth 1 -type f -empty)
if [ -n "$FRAG_LIST" ]; then
    for full_path in $FRAG_LIST; do
        file=$(basename "$full_path")
        if [ "$file" != "$SAFE_CORE_1" ] && [ "$file" != "$SAFE_CORE_2" ] && [ "$file" != "$SAFE_ENGINE" ]; then
            echo "[!] SAFE ZONE CLEAN: Purging $file..."
            dd if=/dev/zero of="$file" bs=1 count=10 2>/dev/null
            rm -f "$file"
        fi
    done
fi
echo "------------------------------------------------------------"
echo "[*] EXECUTING AUTOMATED INTEGRITY CHECKS..."
if [ -f "$SAFE_CORE_1" ] && [ -f "$SAFE_CORE_2" ]; then
    echo "[+] RUNNING DIRECTORY STATE:"
    ls -l "$SAFE_CORE_1" "$SAFE_CORE_2"
    echo "[+] CRYPTOGRAPHIC SIGNATURES (SHA-256):"
    sha256sum "$SAFE_CORE_1" "$SAFE_CORE_2"
fi
echo "============================================================"
