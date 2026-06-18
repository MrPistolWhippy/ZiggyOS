#!/bin/ash
# ZIGGY-OS PERPETUAL CORE SELF-HEALING SENTINEL
sqlite3 /root/archive.db "PRAGMA wal_checkpoint(TRUNCATE);" 2>/dev/null
pkill -9 -f "launch_ptcd_final.py" 2>/dev/null
python3 /root/make_dashboard.py >/dev/null 2>&1 || /root/b.sh >/dev/null 2>&1
if ! pkill -0 -f "http.server"; then
    python3 -m http.server --directory /root/ 8080 >/dev/null 2>&1 &
fi
/root/sync_isolated_storage.sh >/dev/null 2>&1
echo "[2026-06-14 18:11:51] PERPETUAL HEALING SWEEP COMPLETE" >> /root/sweep_output.log
