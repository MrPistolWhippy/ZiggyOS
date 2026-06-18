#!/bin/bash
# ==============================================================================
#  ZIGGYOS AUTOMATED STRING SHIPPER (SSH PIPELINE)
# ==============================================================================

# 1. Capture user string argument
STRING_CONTENT="$1"
TARGET_FILE="config/telemetry_string.txt"

if [ -z "$STRING_CONTENT" ]; then
    echo -e "\033[91m[!] Usage Error: Provide a text string to push. Example:\033[0m"
    echo -e "    git_push_string \"Your text string here\""
    exit 1
fi

echo -e "\033[94m[*] Spawning secure SSH environment layers...\033[0m"
eval "$(ssh-agent -s)" > /dev/null
ssh-add /root/.ssh/id_ed25519 2>/dev/null

# 2. Append or write string to target tracking asset file
echo "$STRING_CONTENT" > "/root/$TARGET_FILE"
echo -e "\033[92m[+] String staged locally inside $TARGET_FILE\033[0m"

# 3. Stage, commit, and push straight to main branch
cd /root
git add "$TARGET_FILE"
git commit -m "Network telemetry update: Auto-shipped live string sequence" > /dev/null

echo -e "\033[94m[*] Transmitting string straight to cloud repository via SSH...\033[0m"
git push origin main

echo -e "\033[92m[+] PIPELINE STREAM COMPLETE. String successfully secured on GitHub!\033[0m"
