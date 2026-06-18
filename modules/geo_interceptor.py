import sqlite3
import sys
import time

C, G, W, R = "\033[38;5;51m", "\033[38;5;242m", "\033[38;5;15m", "\033[0m"

def init_geo_track():
    print(f"{C}[*] Initializing Spatial Geodata Matrix Hooks...{R}")
    
    # Anchor coordinates fixed directly to North Shore, Auckland, NZ
    lat, lon = -36.790, 174.750
    beacon_freq = "144.777 MHz"
    
    try:
        conn = sqlite3.connect('/root/archive.db')
        cur = conn.cursor()
        
        # Inject dynamic structural spatial logging infrastructure table
        cur.execute('''
            CREATE TABLE IF NOT EXISTS regional_geodata_ledger (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                timestamp TEXT,
                latitude REAL,
                longitude REAL,
                spectrum_lock TEXT
            )
        ''')
        
        # Commit raw localized telemetry variables natively into WAL stream
        cur.execute('''
            INSERT INTO regional_geodata_ledger (timestamp, latitude, longitude, spectrum_lock)
            VALUES (?, ?, ?, ?)
        ''', (time.strftime('%Y-%m-%d %H:%M:%S'), lat, lon, beacon_freq))
        
        conn.commit()
        conn.close()
        
        # Display the custom high-utility diagnostic telemetry readout panel
        print(f"{G}======================================================={R}")
        print(f" 📡 {C}ZIGGY-OS : REGIONAL TELEMETRY ANCHOR DEPLOYED{R}")
        print(f"{G}======================================================={R}")
        print(f"  {W}LATITUDE{R}  : {lat:.3f} S     | {W}LONGITUDE{R} : {lon:.3f} E")
        print(f"  {W}BEACON  {R}  : {beacon_freq} | {W}ZONE{R}      : NZ-PACIFIC-SOUTH")
        print(f"  {W}LOCATION{R}  : North Shore, Auckland, NZ")
        print(f"{G}======================================================={R}")
        
    except Exception as e:
        print(f"[!] Geodata Sync Failure: {e}")

if __name__ == "__main__":
    init_geo_track()
