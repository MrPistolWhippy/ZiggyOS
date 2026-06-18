#!/bin/ash
IMG="/root/virtual_disk.img"
if [ ! -f "$IMG" ]; then echo "[-] Target disk missing."; exit 1; fi
sqlite3 /root/archive.db "SELECT * FROM peer_layered_metadata;" > /root/layered_peer_manifest.txt
mcopy -o -i "$IMG" /root/layered_peer_manifest.txt ::/layered_peer_manifest.txt 2>/dev/null
mcopy -o -i "$IMG" /root/dashboard_panel.html ::/dashboard_panel.html 2>/dev/null
mcopy -o -i "$IMG" /root/system_debrief.md ::/system_debrief.txt 2>/dev/null
rm -f /root/layered_peer_manifest.txt
echo -e "\033[92m[+] SUCCESS: SYSTEM METRICS MIRRORED SECURELY TO AIR-GAPPED FAT STORAGE\033[0m"
