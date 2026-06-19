#!/usr/bin/env bash
# ---------------------------------------------------------------------
# BASH NETWORK ENGINE: CRYPTOGRAPHIC SIGNED UDP LISTENER DAEMON
# ---------------------------------------------------------------------
set -uo pipefail

LISTEN_PORT=7003
VALIDATION_TOKEN="STARDUST_NODE_P20_SECRET_AUTH_KEY"

echo -e "\033[0;36m[*] BASH SECURE BRIDGE: Active on UDP channel port ${LISTEN_PORT}...\033[0m"
echo "[*] Enforcing bitwise boundary parsing and signature verification sweeps..."
echo "---------------------------------------------------------------------"

# Open a persistent UDP listener pipeline using netcat
nc -l -u -p "$LISTEN_PORT" | while read -r raw_packet; do
    # Ensure packet contains data bytes to process
    if [ -n "$raw_packet" ]; then
        # Separate the raw incoming packet stream into its component variables
        # Expected structure: [Payload String Data]||[32-Byte Hex Hash Signature]
        payload="${raw_packet%??*}"
        received_sig="${raw_packet##* }"

        # Compute the expected cryptographic signature metric natively in memory memory
        expected_sig=$(echo -n "${payload}${VALIDATION_TOKEN}" | sha256sum | awk '{print $1}')

        if [ "$received_sig" == "$expected_sig" ]; then
            echo -e "[$(date +%H:%M:%S)] \033[0;32m[VERIFIED]\033[0m Shard accepted from peer link. Size: ${#payload} bytes."
            echo "    Parsed Payload: $payload"
        else
            echo -e "[$(date +%H:%M:%S)] \033[0;31m[DROPPED]\033[0m Invalid signature matrix parameter detected."
        fi
    fi
done
