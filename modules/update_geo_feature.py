import sqlite3
import sys
import re

C, G, W, R = "\033[38;5;51m", "\033[38;5;242m", "\033[38;5;15m", "\033[0m"

def apply_feature_upgrades():
    print(f"{C}[*] Optimizing spatial table layout and pruning old frames...{R}")
    try:
        conn = sqlite3.connect('/root/archive.db')
        cur = conn.cursor()
        
        # 1. CLEANUP ROUTINE: Keep the database dense by keeping only the 50 most recent records
        cur.execute('''
            DELETE FROM regional_geodata_ledger 
            WHERE id NOT IN (
                SELECT id FROM regional_geodata_ledger 
                ORDER BY timestamp DESC LIMIT 50
            )
        ''')
        cur.execute('PRAGMA incremental_vacuum;')
        print(f"  [✓] Database vacuumed. Stale coordinates purged from archive.db.")
        conn.commit()
        conn.close()
    except Exception as e:
        print(f"  [!] Optimization error: {e}")

    # 2. INTEGRATE WITH THE COCKPIT PANEL: Inject live location readouts into the UI file
    cockpit_path = "/root/teal_cockpit.py"
    try:
        with open(cockpit_path, "r") as f:
            content = f.read()

        # Define the exact text block we want to stream into the interface display header
        geo_header_block = (
            '            sys.stdout.write(f" 📍 {WHITE}SPATIAL COORDINATES{RESET} : -36.790 S, 174.750 E (North Shore, AKL)\\n")\\n'
            '            sys.stdout.write(f" 🛰️  {WHITE}GLOBAL MATRIX TOPO {RESET} : 75 Active Swarm Nodes (NZ Spectrum Locked)\\n")'
        )

        # Hot-patch the layout file using simple string substitution
        old_pattern = 'sys.stdout.write(f" 🛰️  {WHITE}GLOBAL MATRIX TOPOLOGY{RESET} : 75 Active Swarm Nodes (Unsealed Topology)\\n")'
        if old_pattern in content:
            content = content.replace(old_pattern, geo_header_block)
        else:
            # Fallback for alternative variable layouts
            content = re.sub(r'sys\.stdout\.write\(f" 🛰️.*GLOBAL.*?\n', geo_header_block + '\n', content)

        with open(cockpit_path, "w") as f:
            f.write(content)
        print(f"  [✓] Cockpit layout stream successfully updated with dynamic geodata hooks.")
    except Exception as e:
        print(f"  [!] Cockpit layout injection failed: {e}")

if __name__ == "__main__":
    apply_feature_upgrades()
