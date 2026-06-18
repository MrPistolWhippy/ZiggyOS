#!/bin/sh
cd ~/ishfiles/ziggy-kernel-os
mkdir -p .git
U="https://github.com"
R="MrPistolWhippy/ZiggyOS.git"
echo '[remote "origin"]' > .git/config
echo "url = $U/$R" >> .git/config
echo "fetch = +refs/heads/*:refs/remotes/origin/*" >> .git/config
echo '[branch "main"]' >> .git/config
echo "remote = origin" >> .git/config
echo "merge = refs/heads/main" >> .git/config
echo "nameserver 8.8.8.8" > /etc/resolv.conf
git push origin main
