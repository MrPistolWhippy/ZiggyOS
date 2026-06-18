import sys, time, hashlib, os
H = lambda t: hashlib.sha256(t.encode()).hexdigest()[:16].upper()
h = H(str(time.time()))
C, G, P, Y, W, R = "\033[38;5;51m", "\033[38;5;46m", "\033[38;5;93m", "\033[38;5;220m", "\033[38;5;15m", "\033[0m"
sys.stdout.write("\033[H\033[J")
print(f"{C}⚡ ZIGGY-OS : MASTER-RING-0 {Y}0x{h}{R}\n"+"-"*54)
TRK = [
    ("A", "track_alpha", "Core/WAL", G), 
    ("B", "track_beta", "Manhattan", C), 
    ("C", "track_charlie", "Git Upstream", Y), 
    ("D", "track_delta", "Cold Mirror", P),
    ("E", "track_sammy", "Epsilon Core", W)
]
for n, p, d, col in TRK:
    os.makedirs(f"/root/{p}", exist_ok=True)
    print(f" 🛰️  {col}TRACK_{n}{R} │ /root/{p:<13} │ {d}")
print("-"*54)
try: open("/root/matrix_ledger.hex", "a").write(f"SIG:0x{h}\n")
except: pass
