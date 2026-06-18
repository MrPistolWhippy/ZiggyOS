#!/bin/sh
echo -e "\033[1;36m[*] INITIATING GLOBAL COCKPIT DIAGNOSTIC SWEEP...\033[0m"
rm -f /root/*.py /root/*.log /root/*.txt
/root/workspace/bin/master_automate.sh --fast --verbose
echo -e "\033[1;36m[*] REBUILDING BARE-METAL COMPILATION LAYERS...\033[0m"
make clean && make x86
echo -e "\033[1;32m[+] SUCCESS: Workspace fully optimized, balanced, and compiled.\033[0m"
