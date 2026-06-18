#!/bin/ash
# ZIGGY-OS DISTRIBUTED MESH - RED HAT STYLE SQL TOOLBELT
DB="/root/archive.db"
TEAL="\033[96m"
WHITE="\033[97m"
RESET="\033[0m"

if [ ! -f "$DB" ]; then
    echo -e "[-] Error: Target database asset not found at $DB"
    exit 1
fi

case "$1" in
    "peer-layers")
        echo -e "${TEAL}[+] RELATIONAL DEEP MAP: GEOGRAPHIC & ARCHITECTURE PEER LAYERS:${RESET}"
        sqlite3 "$DB" ".headers on" ".mode column" "SELECT t.us_region, m.peer_id, m.hw_arch, m.layer_crypto_sig AS 'Crypto Sig', m.tracking_channel AS 'Channel' FROM peer_shard_topology t JOIN peer_layered_metadata m ON t.peer_id = m.peer_id ORDER BY t.us_region ASC, m.peer_id ASC LIMIT 12;"
        ;;

    "peer-layers")
        echo -e "${TEAL}[+] RELATIONAL DEEP MAP: GEOGRAPHIC & ARCHITECTURE PEER LAYERS:${RESET}"
        sqlite3 "$DB" ".headers on" ".mode column" "SELECT t.us_region, m.peer_id, m.hw_arch, m.layer_crypto_sig AS 'Crypto Sig', m.tracking_channel AS 'Channel' FROM peer_shard_topology t JOIN peer_layered_metadata m ON t.peer_id = m.peer_id ORDER BY t.us_region ASC, m.peer_id ASC LIMIT 12;"
        ;;

    "us-mesh")
        echo -e "${TEAL}[+] EVEN US GEOGRAPHIC PEER GRID SPREAD:${RESET}"
        sqlite3 "$DB" ".headers on" ".mode column" "SELECT us_region, COUNT(*) AS 'Active Peers', SUM(local_weight) AS 'Total Region Weight' FROM peer_shard_topology GROUP BY us_region;"
        ;;

    "nodes")
        echo -e "${TEAL}[+] INTERNAL GLOBAL INGRESS MESH NODES:${RESET}"
        sqlite3 "$DB" ".headers on" ".mode column" "SELECT id, packet_data FROM ingress_network_node ORDER BY id DESC LIMIT 10;"
        ;;

    "ledger")
        echo -e "${TEAL}[+] RECORDED TRANS-BLOCK CRYPTO SEQUENCES:${RESET}"
        sqlite3 "$DB" ".headers on" ".mode column" "SELECT block_index, substr(block_hash, 1, 24) AS hash_segment, eas_uid, status FROM p6_ledger ORDER BY block_index DESC LIMIT 10;"
        ;;
    "telemetry")
        echo -e "${TEAL}[+] PHYSICAL NODE GAMMA TELEMETRY FLIGHT MATRIX:${RESET}"
        sqlite3 "$DB" ".headers on" ".mode column" "SELECT id, altitude_ft, speed_knots, fuel_pct, timestamp FROM node_gamma_telemetry ORDER BY id DESC LIMIT 5;"
        ;;
    "audit")
        echo -e "${TEAL}[+] INTEG_MATRIX INTEGRITY COUNTS:${RESET}"
        TOTAL=$(sqlite3 "$DB" "SELECT COUNT(*) FROM p6_ledger;")
        ALARM=$(sqlite3 "$DB" "SELECT COUNT(*) FROM p6_ledger WHERE status='ALARM_RING_ACTIVE';")
        echo -e "${WHITE}  - Total Blocks Verified: ${TEAL}$TOTAL${RESET}"
        echo -e "${WHITE}  - Active Threat Vectors:  \033[91m$ALARM${RESET}"
        ;;
    "schema")
        echo -e "${TEAL}[+] SYSTEM DATABASE RELATION MAP SCHEMA:${RESET}"
        sqlite3 "$DB" ".schema"
        ;;
    *)
        echo -e "${WHITE}Usage: ./toolbelt.sh {ledger|telemetry|audit|schema}${RESET}"
        ;;
esac
