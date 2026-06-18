#!/bin/sh
echo "=== SYSTEM NET LANE DIAGNOSTIC ==="
ping -c 1 -w 1 8.8.8.8 >/dev/null 2>&1 && echo "WAN STATUS: ONLINE" || echo "WAN STATUS: AIR-GAP"
for d in dns.google flipperzero.one fix-ski.com; do IP=$(nslookup $d 1.1.1.1 2>/dev/null | awk '/Address/ {print $3}' | tail -n 1); [ ! -z "$IP" ] && printf " [+] %-17s -> %s
" "$d" "$IP" || printf " [-] %-17s -> Loopback (127.0.0.1)
" "$d"; done
