import time, subprocess
print("=========================================")
2026 SYSTEM LOGS: INTERNAL CRON SERVICE ACTIVE
=========================================")
try:
    while True:
        timestamp = time.strftime("%H:%M:%S")
        print(f"[{timestamp}] [CRON EVENT] Initializing system data-sweep...")
        
        # Trigger your network-free backup tool directly
        subprocess.run(["python3", "backup.py"])
        
        # Pause execution until the next 60-second block hits
        time.sleep(60)
except KeyboardInterrupt:
    print("\nCron execution halted safely.")
