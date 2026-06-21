#!/bin/bash
# ==============================================================================
#          ZIGGY-OS MONITOR: TFTP TRANSMISSION METRIC TRACKER
# ==============================================================================

echo "[*] STANDING BY FOR BARE-METAL HARDWARE TRANSFER REQUESTS..."
echo "    [Press CTRL+C to exit monitor loop]"
echo "--------------------------------------------------------"

# Baseline packet reader
INITIAL_PACKETS=$(awk '/eth0|wlan0|rmnet0/ {print $2 + $10}' /proc/net/dev 2>/dev/null | awk '{s+=$1} END {print s}')
if [ -z "$INITIAL_PACKETS" ]; then INITIAL_PACKETS=0; fi

while true; do
    # Read current live network throughput data streams
    CURRENT_PACKETS=$(awk '/eth0|wlan0|rmnet0/ {print $2 + $10}' /proc/net/dev 2>/dev/null | awk '{s+=$1} END {print s}')
    if [ -z "$CURRENT_PACKETS" ]; then CURRENT_PACKETS=0; fi
    
    # Calculate ongoing metric variances
    DELTA=$((CURRENT_PACKETS - INITIAL_PACKETS))
    
    clear
    echo "--------------------------------------------------------"
    echo "       ZIGGY-OS LIVE NETWORK METRICS DISPLAY PANEL"
    echo "--------------------------------------------------------"
    echo "  Status: STANDING BY / TRANSMITTING"
    echo "  Cumulative Data Traffic Changes: $DELTA packets"
    echo "--------------------------------------------------------"
    
    if [ "$DELTA" -gt 500 ]; then
        echo "  [⚡ ALERT] Active High-Throughput Stream Detected!"
        echo "  --> Bare-metal board is pulling your verified binary right now."
    fi
    
    sleep 2
done
