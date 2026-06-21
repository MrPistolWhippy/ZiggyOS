#!/bin/bash
# ==============================================================================
#          ZIGGY-OS PIPELINE: AUTOMATED LOCAL VERIFICATION & DEPLOYMENT
# ==============================================================================

TARGET_DB="mesh_topology_ledger.db"
EXPECTED_CID="1389e1c980da15c3fa3096d78c48416d0b6e227f518a2f9abb7c33d2c238d4c0"
TFTP_DIR="/var/tftpboot"
FIRMWARE_SOURCE="system.img"
FIRMWARE_TARGET="ziggy_firmware.img"

echo "[*] TARGET IDENTIFIED: $TARGET_DB"

if [ ! -f "$TARGET_DB" ]; then
    echo "    └── [ERROR] Target ledger file missing from active workspace root."
    exit 1
fi

# Compute the actual SHA-256 validation token
ACTUAL_CID=$(sha256sum "$TARGET_DB" | awk '{print $1}')

if [ "$ACTUAL_CID" = "$EXPECTED_CID" ]; then
    echo "    └── [VERIFIED] Root Storage CID: $ACTUAL_CID"
    echo "[*] INITIATING DEPLOYMENT PIPELINE..."
    
    # Verify TFTP transmission directory exists
    if [ ! -d "$TFTP_DIR" ]; then
        mkdir -p "$TFTP_DIR"
    fi
    
    # Deploy verified image blocks into the active TFTP stream path
    if [ -f "$FIRMWARE_SOURCE" ]; then
        cp "$FIRMWARE_SOURCE" "$TFTP_DIR/$FIRMWARE_TARGET"
        echo "    └── [SUCCESS] Verified runtime image deployed to $TFTP_DIR/$FIRMWARE_TARGET"
        echo "    └── [READY] Standing by for bare-metal board connection requests."
    else
        echo "    └── [WARNING] $FIRMWARE_SOURCE missing. Skipped binary stage transfer."
    fi
else
    echo "    └── [CRITICAL] CID Mismatch! Validation failed."
    echo "        Expected: $EXPECTED_CID"
    echo "        Received: $ACTUAL_CID"
    exit 1
fi
