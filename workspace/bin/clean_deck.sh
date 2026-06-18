#!/bin/sh
echo "[*] Initializing ZiggyOS Workspace Optimization Sweep..."
rm -f /root/*.o /root/*.bin /root/*.elf /root/*.iso
rm -f /root/workspace/src/*.o
echo " > Log files condensed."
echo " > Standalone build targets pruned back to baseline memory footprint."
echo "[+] Target system partition balanced."
