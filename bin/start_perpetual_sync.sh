#!/bin/ash
# ==============================================================================
#   11-HOUR 11-MINUTE CONTEXT AUTOMATION LOOP AND STORAGE COOLDOWN DAEMON
# ==============================================================================
pkill -f "start_perpetual_sync.sh"

while true; do
  # A. Regenerate the Multicolor Cyberpunk HTML panel view directly via Python
  python3 /root/fix_dashboard.py > /dev/null 2>&1
  
  # B. Recompile simple concept knowledge data sheets for Leo
  if [ -f "/root/leo_debrief.py" ]; then
      python3 /root/leo_debrief.py > /dev/null 2>&1
  fi
  
  # C. Mirror all updated visual files straight to the virtual FAT volume partition image
  /root/sync_isolated_storage.sh > /dev/null 2>&1
  
  # D. High-stability sleep timeout: 11 Hours, 11 Minutes = 40260 Seconds
  sleep 40260
done &
echo -e "\033[92m[+] TWICE-A-DAY SCHEDULER INITIALIZED: BACKGROUND DAEMON STREAM IS ACTIVE\033[0m"
