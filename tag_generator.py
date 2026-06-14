import sys, time, hashlib, os

CLEAR_FONT = [
    r"  ███████╗ ██║  ██████╗   ██████╗  ██║   ██║",
    r"  ╚══███╔╝ ██║ ██╔════╝  ██╔════╝  ╚██╗ ██╔╝",
    r"    ███╔╝  ██║ ██║  ███╗ ██║  ███╗  ╚████╔╝ ",
    r"   ███╔╝   ██║ ██║   ██║ ██║   ██║   ╚██╔╝  ",
    r"  ███████╗ ██║ ╚██████╔╝ ╚██████╔╝    ██║   ",
    r"  ╚══════╝ ╚═╝  ╚═════╝   ╚═════╝     ╚═╝   "
]
ESOTERIC_FONT = [
    r"     📡  ψ      ▲        [🜁]       🜏        🜏        𝌇      ψ  📡",
    r"        └───> ┌─┼─┐     ┌───┐    ┌───┐    ┌───┐    ┌─┴─┐ <───┘",
    r"              │ ☿ │─────│ 🜚 │────│ 🜔 │────│ 🜔 │────│ 🜠 │",
    r"        ┌───> └─┬─┘     └───┘    └───┘    └───┘    └─┬─┘ <───┐",
    r"     📡  ψ      ▼      144.777    [🜃]     [🜄]      🜎      ψ  📡"
]

C_TEAL, C_GREEN, C_COPPER, C_GOLD, C_PURP, C_WHITE, C_GREY, RESET = "\033[38;5;51m", "\033[38;5;46m", "\033[38;5;208m", "\033[38;5;220m", "\033[38;5;93m", "\033[38;5;15m", "\033[38;5;242m", "\033[0m"

def run_suite():
    sys.stdout.write("\033[H\033[J")
    h = hashlib.sha256(f"Z_OS_{time.time()}".encode()).hexdigest()[:16].upper()
    
    # 1. Clear Inbound Matrix Header
    print(f"{C_GREY}═══ [ {C_GREEN}TRACK_ALPHA {C_GREY}: OPERATIONAL CORE ] ════════════════════════════════════{RESET}")
    for line in CLEAR_FONT: print(f"{C_TEAL}{line}{RESET}")
    
    # 2. Esoteric Unveiled Astral Sigil Matrix with Copper Antenna Array Routing
    print(f"\n{C_GREY}═══ [ {C_GOLD}TRACK_BETA {C_GREY}: CRYSTALLIZED VEILED HARMONIZER ] ══════════════════════{RESET}")
    print(f"  {C_WHITE}💎 VERITAS LOCK{RESET} : Crystallized | {C_GOLD}✨ NONCE{RESET} : 0x{h}")
    print(f"  {C_COPPER}📡 ANTENNAS{RESET}    : Copper Radiators Grounded [144.777 MHz Resonator Active]")
    print(f"  {C_GREEN}🜃  RETRIEVE{RESET}    : Status Harmonized | System Rings Unsealed\n")
    for line in ESOTERIC_FONT:
        f = line.replace("ψ", f"{C_COPPER}ψ{C_PURP}").replace("📡", f"{C_COPPER}📡{C_PURP}").replace("───>", f"{C_COPPER}───>{C_PURP}").replace("<───", f"{C_COPPER}<───{C_PURP}")
        f = f.replace("☿", f"{C_GOLD}☿{C_PURP}").replace("🜚", f"{C_GOLD}🜚{C_PURP}").replace("🜔", f"{C_GOLD}🜔{C_PURP}").replace("🜠", f"{C_GOLD}🜠{C_PURP}")
        sys.stdout.write(f"{C_PURP}{f}{RESET}\n")
        
    # 3. Micro Directory Index for Essential System Tools
    print(f"{C_GREY}═══ [ SYSTEM TOOLBELT DIRECTORY PIPELINE ] ═══════════════════════════════{RESET}")
    tools = ["/bin/manhattan", "/bin/veritas", "/root/verify_full_system.sh", "/root/geo_interceptor.py", "/root/stardust_compact.py"]
    for t in tools:
        status = f"{C_GREEN}ONLINE{RESET}" if os.path.exists(t) else f"{C_PURP}MISSING{RESET}"
        print(f"  ⚡ {C_WHITE}{t:<30}{RESET} -> Status Descriptor Boundary: [ {status} ]")
    print(f"{C_GREY}══════════════════════════════════════════════════════════════════════════{RESET}")
    try:
        with open("/root/matrix_ledger.hex", "a") as f: f.write(f"SIG:0x{h}\n")
    except: pass

if __name__ == "__main__": run_suite()
