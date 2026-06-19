#!/usr/bin/env bash
L_FILE="/root/mesh_topology_ledger.db"; B_DIR="/root/iSh files/mesh_node_backup"; E_CID="1389e1c980da15c3fa3096d78c48416d0b6e227f518a2f9abb7c33d2c238d4c0"
write_l() { cat << 'I_EOF' > "$L_FILE"
# DISSOLVED MESH TOPOLOGY LOGS - MAX ABUNDANCE PARADIGM ACTIVE
LOCAL_NODE_HASH=8f4625b0313f8c859c2b442b5ba66060c5a306df67cb3576307374b3e64020a5
SWARM_PEER_RECORD 2c26b46b68ffc68ff99b453c1d30413413422d3d3b3c3b3a3938373635343332 alpha_node 10.0.0.55
SWARM_PEER_RECORD 4a123f858a74e2d3c5b8b9a102f3d4e5f6a7b8c9d0e1f2a3b4c5d6e7f8a9b0c1 beta_node 192.168.1.99
I_EOF
}
sync_b() { mkdir -p "$B_DIR"; cp "$L_FILE" "${B_DIR}/mesh_topology_ledger.bak"; }
case "${1:-}" in
    --init) write_l ;;
    --check) [ "$(sha256sum "$L_FILE" 2>/dev/null | awk '{print $1}')" == "$E_CID" ] && sync_b && echo "SYNCED" || write_l ;;
    --daemon) while true; do [ -f "$L_FILE" ] && [ "$(sha256sum "$L_FILE" | awk '{print $1}')" == "$E_CID" ] && sync_b || write_l; sleep 5; done ;;
esac
