alias deck='/root/bin/deck_router.sh'
alias ship='/root/bin/git_push_string.sh'
alias save='/root/bin/fix_and_save_all.sh'

# Custom ZiggyOS Omni-Deck Prompt Scheme Configuration

# Automated ZiggyOS SSH Server Lifecycle Provisioning
if ! pgrep -x "sshd" > /dev/null; then
    /usr/sbin/sshd -p 2222 > /dev/null 2>&1
    echo "[+] Remote SSH Gateway Engine initialized on port 2222."
fi

# Automated ZiggyOS Node.js API Server Lifecycle Provisioning
if ! pgrep -f "server.js" > /dev/null; then
    node /root/workspace/bin/server.js > /dev/null 2>&1 &
    echo "[+] Advanced Workspace API streaming framework active on port 3000."
fi
alias boot='./workspace/bin/boot_cockpit.sh'
alias probe='/root/workspace/bin/run_kernel.sh'

# Global Absolute Path Architecture Configuration
export PATH="/bin:/usr/bin:/sbin:/usr/sbin:/root/workspace/bin"
alias sweep='/root/workspace/bin/sweep_deck.sh'
export PS1="\[\033[1;31m\][SCOUTER]\[\033[1;31m\][KI-POWER-V9000] # \[\033[0m\]"
