import sys, os, random, time
p, vfs = "/sys/kernel/bin", {"/": ["sys", "dev", "usr"], "/sys": ["kernel", "bin"], "/sys/kernel": ["bin"], "/sys/kernel/bin": ["kernel.bin (4.2 MB)"], "/dev": ["uart0", "mem"], "/usr": ["home"]}
f, t, tk, up_t, pll, pull = 1.20, 38.2, 0, time.time(), True, True
print("=== ZIGGYOS EMBEDDED ARCHITECTURE v6.0-NATIVE ===\n🛡️ SAFEGUARDS ACTIVE: CLOCK PLL, ALIGNMENT, PULL-UP.\n")
while True:
 try:
  if not pll: print("\n\033[1;31m[💥 CLOCK FAULT] PLL UNLOCKED! Falling back to internal RC (8MHz)...\033[0m"); f, t, pll = 0.008, 42.1, True
  sys.stdout.write(f"ziggy-sh:{p}> "); sys.stdout.flush(); r = sys.stdin.readline().strip().lower()
  if not r: continue
  pt = r.split(None, 1); cmd = pt if pt else ""; arg = pt if len(pt) > 1 else ""
  cmd = "rabbit" if "rabb" in r or "hexx" in r else "boot" if "boot" in cmd or "kern" in cmd else cmd
  if cmd == "help": print("help, status, boot, cd, ls, overclock, toggle-pull, trigger-fault, exit")
  elif cmd in ["vfs", "ls"]: [print(f"  └── {i}") for i in vfs.get(p, [])]
  elif cmd == "cd":
   if not arg or arg == "~": p = "/sys/kernel/bin"
   elif arg == "..": p = "/sys/kernel" if p == "/sys/kernel/bin" else "/sys" if p == "/sys/kernel" else "/"
   else: tp = f"{p if p != '/' else ''}/{arg}"; p = tp if tp in vfs else p
  elif cmd == "status": print(f"  Freq: {f:.3f}GHz | Temp: {t:.1f}C | Pull-up: {'ENABLED' if pull else 'FLOATING NOISE!'}\n  PLL Lock: {pll} | Uptime: {int(time.time() - up_t)}s")
  elif cmd == "toggle-pull": pull = not pull; print(f"[*] GPIO Pin pull-up is now {'ENABLED' if pull else 'DISABLED'}.")
  elif cmd == "trigger-fault": pll = False
  elif cmd == "overclock":
   tgt = 0x0800C000 + random.randint(0, 2000); print(f"[*] Flash alignment target: 0x{tgt:08X}")
   if tgt > 0x0800C7FF: print("\n\033[1;31m[💥 HARDFAULT] MEMORY BOUNDARY BREACHED! Partition Isolated.\033[0m")
   else: f, t = 5.40, 89.4; print("[🚀] OVERCLOCK SPEED ACTIVE: 5.40 GHz.")
  elif cmd == "boot":
   print("[*] Running Scheduler...\n--- Live Output ---"); st, ct = time.time(), 0
   while time.time() - st < 2.0:
    if not pull and random.random() < 0.4: print("\n\033[1;33m[⚡ NOISE EVENT] Floating Pin Interrupt Lockup!\033[0m"); time.sleep(0.2)
    sys.stdout.write(f"\r  [TASK {ct+1}] Heap: {random.randint(200,900)}/1024B | Clock: {f:.3f}GHz"); sys.stdout.flush()
    ct = (ct + 1) % 3; time.sleep(0.08)
   print("\n[*] Scheduler completed.")
  elif cmd == "rabbit":
   print("\n\033[1;35m[🔮] THE HIGH PRIESTESS UNSEALS SECTOR 0..."); [print(f"  0x{i*16:08X}: " + " ".join(f"{random.randint(64,126):02X}" for _ in range(16))) for i in range(4)]; print("\033[1;36m[🐇] \"Sector 0 decoded cleanly on iPad architecture.\"\033[0m\n")
  elif cmd == "exit": break
  else: print(f"unmapped: {cmd}")
 except KeyboardInterrupt: print("\nUse 'exit'.")
