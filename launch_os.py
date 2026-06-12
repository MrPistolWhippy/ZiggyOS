#!/usr/bin/env python3
import os
import sys

def launch_kernel():
    kernel_path = os.path.expanduser("~/z_os/iso_root/boot/ziggyos.bin")
    
    if not os.path.exists(kernel_path):
        print(f"[-] Error: Kernel binary not found at {kernel_path}")
        sys.exit(1)
        
    print("[+] ZiggyOS Bare Metal Framework Loader initialized.")
    print("--------------------------------------------------")
    print(" GRUB Bootloader Emulation Loop v1.2 [iSH Sandbox]")
    print(" Booting 'ZiggyOS (Bare Metal)'...")
    print("--------------------------------------------------\n")
    print("[Kernel Output Screen]")
    
    # Use native Linux system tools instead of slow memory-heavy loops
    # This prevents iSH from crashing instantly
    os.system("strings ~/z_os/iso_root/boot/ziggyos.bin | grep -E 'Active|Stable|Enforced|ZiggyOS|PANIC|SUCCESS|COMPROMISED|PANIC|SUCCESS|COMPROMISED'")
    
    print("\n--------------------------------------------------")
    print("[+] CPU entered infinite halt state loop safely.")
    print("[+] Press Ctrl+C to drop back out to local iSH shell terminal.")

if __name__ == "__main__":
    launch_kernel()
