#!/usr/bin/env bash
set -uo pipefail
LISTEN_PORT=7003
VALIDATION_TOKEN="STARDUST_NODE_P20_SECRET_AUTH_KEY"

echo -e "\033[0;36m[*] BASH SECURE BRIDGE: Listening on port ${LISTEN_PORT}...\033[0m"
echo "---------------------------------------------------------------------"

while true; do
    # Correct iSH Flag Pairing: Bind port and capture a single incoming data frame safely
    raw_packet=$(nc -lp $LISTEN_PORT 2>/dev/null)
    if [ -n "$raw_packet" ]; then
        payload="${raw_packet% *}"
        received_sig="${raw_packet##* }"
        expected_sig=$(echo -n "${payload}${VALIDATION_TOKEN}" | sha256sum | awk '{print $1}')
        
        if [ "$received_sig" == "$expected_sig" ]; then
            echo -e "[$(date +%H:%M:%S)] \033[0;32m[VERIFIED]\033[0m Payload: $payload"
        else
            echo -e "[$(date +%H:%M:%S)] \033[0;31m[DROPPED]\033[0m Invalid signature parameter."
        fi
    fi
    sleep 0.5 # Safe watchdog throttle protects the mobile app text rendering buffer
done
