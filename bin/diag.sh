#!/bin/sh

# Beautiful Terminal Styling Profiles
BLUE='\033[0;34m'
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
RESET='\033[0m'

echo "==============================================="
echo "   ZIGGYOS BARE-METAL KERNEL DIAGNOSTIC SYSTEM "
echo "==============================================="

# 1. Check Workspace Environment Path Structure
echo -e "\n${BLUE}[*] Auditing source directories...${RESET}"
if [ -d "src" ]; then
    C_FILES=$(ls src/*.c 2>/dev/null | wc -l)
    S_FILES=$(ls src/*.s 2>/dev/null | wc -l)
    echo -e "  - ${GREEN}Found src/ folder layout!${RESET}"
    echo "  - High-level modules (*.c): $C_FILES files"
    echo "  - Assembly vectors   (*.s): $S_FILES files"
else
    echo -e "  - ${RED}CRITICAL: Missing src/ system root folder!${RESET}"
fi

# 2. Track Binary Compilation Target Offsets
echo -e "\n${BLUE}[*] Auditing compiled image architectures...${RESET}"
TARGETS="os-kernel.bin ziggyos.bin ziggyos.elf"
FOUND_BIN=0

for target in $TARGETS; do
    if [ -f "$target" ]; then
        SIZE=$(ls -lh "$target" | awk '{print $5}')
        echo -e "  - Target ${GREEN}$target${RESET} exists! Size: ${YELLOW}$SIZE${RESET}"
        FOUND_BIN=1
    else
        echo "  - Target $target: Not found in this workspace path branch"
    fi
done

if [ $FOUND_BIN -eq 0 ]; then
    echo -e "  - ${RED}WARN: No target executable images compiled locally.${RESET}"
fi

# 3. Verify System Makefile Tabs Structural Security
echo -e "\n${BLUE}[*] Auditing Makefile rule validation...${RESET}"
if [ -f "Makefile" ]; then
    # Scan for true system tabs (\t) inside the Makefile target rules
    grep -q $'\t' Makefile
    if [ $? -eq 0 ]; then
        echo -e "  - ${GREEN}Makefile syntax check PASSED! Perfect native tabs detected.${RESET}"
    else
        echo -e "  - ${RED}CRITICAL WARNING: Spaces detected in Makefile recipe lines! Run tabs-printf script.${RESET}"
    fi
else
    echo "  - No local Makefile declared in this branch path."
fi

# 4. Check Tracking Tree Registry Status
echo -e "\n${BLUE}[*] Auditing active git workspace tags...${RESET}"
UNTRACKED=$(git status --porcelain | grep "??" | wc -l)
MODIFIED=$(git status --porcelain | grep "M" | wc -l)
echo "  - Uncommitted modified trackers: $MODIFIED files"
echo "  - Red untracked storage clutter: $UNTRACKED files"

echo -e "\n==============================================="
echo -e "   DIAGNOSTIC CYCLE COMPLETED SUCCESSFULLY     "
echo -e "==============================================="
