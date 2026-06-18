alias deck='/root/bin/deck_router.sh'
alias ship='/root/bin/git_push_string.sh'
alias save='/root/bin/fix_and_save_all.sh'

# Custom ZiggyOS Omni-Deck Prompt Scheme Configuration
export PS1="\[\033[1;35m\][\[\033[1;36m\]ZIGGYOS\[\033[1;35m\]]\[\033[1;32m\][DECK-V180.0]\[\033[1;33m\] # \[\033[0m\]"

# Automated ZiggyOS SSH Server Lifecycle Provisioning
if ! pgrep -x "sshd" > /dev/null; then
    /usr/sbin/sshd -p 2222 > /dev/null 2>&1
    echo "[+] Remote SSH Gateway Engine initialized on port 2222."
fi
