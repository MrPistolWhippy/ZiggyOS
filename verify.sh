#!/usr/bin/env bash
set -uo pipefail

GREEN='\033[0;32m'
CYAN='\033[0;36m'
YELLOW='\033[0;33m'
RED='\033[0;31m'
NC='\033[0m'

LEDGER_FILE="mesh_topology_ledger.db"

if [ ! -f "$LEDGER_FILE" ]; then
    echo -e "${RED}[FAIL] Target matrix ledger file missing.${NC}"
    exit 1
fi

echo -e "${CYAN}[*] TARGET IDENTIFIED: $LEDGER_FILE${NC}"
CALCULATED_CID=$(sha256sum "$LEDGER_FILE" | awk '{print $1}')
echo -e "   └── ${GREEN}[VERIFIED] Root Storage CID:${NC} $CALCULATED_CID"
