import sys, time, random, hashlib

C, G, Y, W, R = "\033[38;5;51m", "\033[38;5;46m", "\033[38;5;220m", "\033[38;5;15m", "\033[0m"

def print_local_document(job_name, content_string):
    print(f"\n{C}⚡ ZIGGY-OS : LOCAL OFF-GRID LASER PRINT TECH SPOOLER v1.0{R}")
    print(f" \033[38;5;242m" + "─"*66 + f"{R}")
    
    job_id = random.randint(1000, 9999)
    h_job = hashlib.sha256(content_string.encode()).hexdigest()[:8].upper()
    
    print(f"  [🖨️] DEVICE CHANNEL │ Bare-Metal Local Laser Printer Interconnect")
    print(f"  [📋] SPOOL JOB ID   │ #LP-{job_id} │ Document Tag: {job_name}")
    print(f"  [⚡] DRIVER EMULATE │ Raw PostScript/PCL Translation Engine Active")
    print(f"  [🔌] ACCESS MEDIUM  │ Wired Port Data Link │ Status: [ AIR-GAPPED SAFE ]")
    print(f"  [📜] RENDERING DATA │ \"\033[38;5;15m{content_string}\033[0m\"")
    
    # Simulate high-velocity laser drum electrostatic fuse duration
    time.sleep(0.5)
    print(f"  [✓] PRINTER STATUS │ Status: [ {G}PRINT JOB FIRED SUCCESSFULLY │ DRUM LINE ACTIVE{R} ]")
    print(f" \033[38;5;242m" + "─"*66 + f"{R}")
    
    try:
        open("/root/matrix_ledger.hex", "a").write(f"SIG:0xLASER_PRINT_JOB_{h_job}_OK\n")
    except: pass

if __name__ == "__main__":
    if len(sys.argv) > 2:
        print_local_document(sys.argv[1], sys.argv[2])
    else:
        print_local_document("ALLIANCE_MANIFEST", "MrPistolWhippy ⚔️ Scribe Man ⚔️ PISTOL_SCRIBE_SSJ4 Core")
