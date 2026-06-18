#!/bin/sh
cd ~/ishfiles/ziggy-kernel-os

# 1. Remove the duplicate definition of system_jiffies_ticks from main.c
sed -i '/uint32_t system_jiffies_ticks = 0;/d' src/main.c

# 2. Add the missing app_diag_main stub to main.c so the linker finds the reference
cat << 'STB' >> src/main.c

void app_diag_main(void) {
    // Diagnostic entry placeholder
    return;
}
STB

# 3. Commit and push the updates directly
git commit -am "Fix system_jiffies_ticks duplicate and provide app_diag_main stub"
git push origin main
