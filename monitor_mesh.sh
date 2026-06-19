#!/usr/bin/env bash

# Local path allocation parameters
TARGET_FILE="/root/mesh_topology_ledger.db"
EXPECTED_CID="1389e1c980da15c3fa3096d78c48416d0b6e227f518a2f9abb7c33d2c238d4c0"

# Terminal interface color maps
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
RED='\033[0;31m'
NC='\033[0m'

echo -e "${GREEN}[*] INITIALISING PERSISTENT MONITORING DAEMON ON NODE p20...${NC}"
echo "Monitoring Target: $TARGET_FILE"
echo "Expected Base Hash: $EXPECTED_CID"
echo "Press [CTRL+C] to stop the daemon safely."
echo "---------------------------------------------------------------------"

# Continuous state evaluation engine
while true; do
    if [ ! -f "$TARGET_FILE" ]; then
        echo -e "${RED}[ALERT] Critical Fault: Topology ledger dropped from path layout!${NC}"
    else
        # Compute real-time hash state
        CURRENT_CID=$(sha256sum "$TARGET_FILE" | awk '{print $1}')
        
        if [ "$CURRENT_CID" == "$EXPECTED_CID" ]; then
            echo -e "[$(date +%H:%M:%S)] ${GREEN}[MATCH]${NC} State is immutable. Map aligns with base parameters."
        else
            echo -e "[$(date +%H:%M:%S)] ${RED}[DRIFT]${NC} Data variance detected! New Map Hash: $CURRENT_CID"
            echo -e "${YELLOW}[!] Action required: Verify external network syncing nodes.${NC}"
        fi
    fi
    # Throttle interval (5 seconds) protects the mobile device battery and memory buffer
    sleep 5
done
