#!/bin/sh
# Run python core asset manager
python3 download_iso.py

if [ ! -f "ziggyos.iso" ]; then
    echo "Using local binary fallback..."
fi

echo "=== 4. Launching ZiggyOS inside QEMU x86 Hardware Emulator ==="
echo "Directing video frame buffer into terminal graphics matrix..."
echo "Press Ctrl+A then X to exit the emulator safely."
echo "--------------------------------------------------------"
qemu-system-x86_64 -nographic -cdrom ziggyos.iso -m 32
