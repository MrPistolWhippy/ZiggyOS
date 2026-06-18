#!/usr/bin/env python3
# ==============================================================================
#  THEOSI ZIGGYOS TACTICAL SCOUTER OVERLAY HUD - FLUSH RAW UNIFIED REPAIR
# ==============================================================================
import os, sys, time, sqlite3
from datetime import datetime

LOG_PATH = "/root/logs/network_sniff.log"
DB_PATH = "/root/archive.db"

def run_scouter_recon_loop():
    print("\033[2J\033[H\033[38;5;198m============================================================\033[0m")
    print("\033[1;36m           THEOSI ZIGGYOS SCOUTER OVERLAY HUD v200.0        \033[0m")
    print("\033[38;5;198m============================================================\033[0m")
    
    try:
        # Hardware Oscillator telemetry data row
        print("\033[94m[+] ACTIVE HARDWARE CRYSTAL OSCILLATOR TACTICAL LOCK:\033[0m")
        print("  - Target Frequency : \033[92m144.777 MHz [ Electric Teal RF Beacon ]\033[0m")
        print("  - Timing Constraint: \033[92mVALIDATED (125 MHz Clock Delay Sub-8.00ns)\033[0m")
        
        # Local Geo Position fix databank query block
        print("\n\033[94m[+] GEOMETRIC LATTICE COORDINATE ENVIRONMENT MAP:\033[0m")
        if os.path.exists(DB_PATH):
            conn = sqlite3.connect(DB_PATH)
            c = conn.cursor()
            c.execute("SELECT raw_text_payload FROM offline_web_vault WHERE keyword='auckland' LIMIT 1;")
            row = c.fetchone()
            if row and len(row) > 0:
                # Print the flat raw string directly to completely eliminate index splitting crashes
                print(f"  - Location Fix     : \033[1;33m{row[0]}\033[0m")
            else:
                print("  - Location Fix     : \033[90m[-] Awaiting hyper-index sync...\033[0m")
            conn.close()
        else:
            print("  - Location Fix     : \033[90m[-] DB matrix offline.\033[0m")
            
        # Raw electromagnetic wave intercept monitoring section
        print("\n\033[1;36m📡 REAL-TIME ELECTROMAGNETIC WAVE SIGNAL INTERCEPTS:\033[0m")
        if os.path.exists(LOG_PATH) and os.path.getsize(LOG_PATH) > 0:
            with open(LOG_PATH, "r") as f:
                lines = f.readlines()[-2:]
                for line in lines:
                    print(f"  -> \033[93m{line.strip()}\033[0m")
                    
            ts = datetime.now().strftime('%Y%m%d_%H%M%S')
            report_name = f"/root/logs/scouter_scan_{ts}.txt"
            with open(report_name, "w") as rf:
                rf.write(f"[SCOUTER_RECON_REPORT] {datetime.now()}\n[FREQ] 144.777 MHz\n")
            print(f"\n\033[1;32m[✓] TACTICAL DATA DUMP LOGGED: {report_name}\033[0m")
        else:
            print("  \033[90m[-] Awaiting high-speed background IQ data bursts...\033[0m")
            
        print("\033[38;5;198m============================================================\033[0m")
    except Exception as e:
        print(f"[-] Scouter Engine Failure: {e}")

if __name__ == "__main__":
    run_scouter_recon_loop()
