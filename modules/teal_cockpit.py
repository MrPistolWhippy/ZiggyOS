#!/usr/bin/env python3
import os, sys
print("\033[2J\033[H\033[1;36mTHEOSI TEAL_COCKPIT LIVE MONITOR\033[0m\n"+"="*50)
if os.path.exists("/root/logs/network_sniff.log"):
    with open("/root/logs/network_sniff.log") as f: print("".join(f.readlines()[-8:]))
else: print("[-] Awaiting streams...")
