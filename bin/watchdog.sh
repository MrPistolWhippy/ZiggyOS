#!/bin/ash
# AUTOMATED RUNTIME WATCHDOG MANAGER
if ! pkill -0 -f launch_ptcd_final.py; then
    echo "[!] Telemetry engine drop detected. Executing recovery boot pass..." >> /root/sweep_output.log
    python3 /root/launch_ptcd_final.py >> /root/sweep_output.log 2>&1 &
fi
