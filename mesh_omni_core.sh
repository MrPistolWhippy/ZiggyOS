#!/usr/bin/env bash
set -uo pipefail

GREEN='\033[0;32m'
CYAN='\033[0;36m'
YELLOW='\033[0;33m'
RED='\033[0;31m'
NC='\033[0m'

LEDGER_FILE="/root/mesh_topology_ledger.db"
# Target the localized, persistent file directory revealed in your ls tree mapping
BACKUP_DIR="/root/iSh files/mesh_node_backup"
EXPECTED_CID="1389e1c980da15c3fa3096d78c48416d0b6e227f518a2f9abb7c33d2c238d4c0"

write_baseline_ledger() {
    cat << 'INNER_EOF' > "$LEDGER_FILE"
# DISSOLVED MESH TOPOLOGY LOGS - MAX ABUNDANCE PARADIGM ACTIVE
LOCAL_NODE_HASH=8f4625b0313f8c859c2b442b5ba66060c5a306df67cb3576307374b3e64020a5
SWARM_PEER_RECORD 2c26b46b68ffc68ff99b453c1d30413413422d3d3b3c3b3a3938373635343332 alpha_node 10.0.0.55
SWARM_PEER_RECORD 4a123f858a74e2d3c5b8b9a102f3d4e5f6a7b8c9d0e1f2a3b4c5d6e7f8a9b0c1 beta_node 192.168.1.99
INNER_EOF
}

trigger_sync_replication() {
    # Force create path with safe string quotes to handle space parameters in folder names
    mkdir -p "$BACKUP_DIR"
    cp "$LEDGER_FILE" "$BACKUP_DIR/mesh_topology_ledger.bak"
}

run_audit_daemon() {
    while true; do
        if [ ! -f "$LEDGER_FILE" ]; then
            write_baseline_ledger
        else
            CURRENT_CID=$(sha256sum "$LEDGER_FILE" | awk '{print $1}')
            if [ "$CURRENT_CID" == "$EXPECTED_CID" ]; then
                trigger_sync_replication
            else
                write_baseline_ledger
            fi
        fi
        sleep 5
    done
}

case "${1:-}" in
    --init)   write_baseline_ledger ;;
    --check)
        CURRENT_CID=$(sha256sum "$LEDGER_FILE" 2>/dev/null | awk '{print $1}') || CURRENT_CID="MISSING"
        if [ "$CURRENT_CID" == "$EXPECTED_CID" ]; then
            trigger_sync_replication
            echo -e "${GREEN}[+] Sync verification complete. Persistent local mirror written.${NC}"
        else
            write_baseline_ledger
        fi
        ;;
    --daemon) run_audit_daemon ;;
esac
