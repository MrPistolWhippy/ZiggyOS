#!/bin/ash
set -e
echo "nameserver 1.1.1.1" > /etc/resolv.conf
echo "nameserver 8.8.8.8" >> /etc/resolv.conf
ip route del default 2>/dev/null || true
ip route add default via 192.168.1.1 2>/dev/null || true
echo "[✔] Network socket configurations flushed."
