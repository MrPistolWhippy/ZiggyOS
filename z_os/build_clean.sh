#!/bin/sh
cd ~/z_os
echo "==> Cleaning messy duplicate versions..."
rm -f src/*[234]*.c src/*copy* *.ld\ [234] build\ [234].zig
echo "==> Gathering latest stable GRUB modules..."
mkdir -p bootloader/i386-pc
LATEST_GRUB=$(ls -td /root/'Ish files'/grub.* 2>/dev/null | head -n 1)
if [ -d "$LATEST_GRUB" ]; then
    cp "$LATEST_GRUB"/boot/grub/i386-pc/*.mod bootloader/i386-pc/
    echo "Successfully cached modules from $(basename "$LATEST_GRUB")"
else
    echo "Warning: No automated GRUB folders found in 'Ish files'"
fi
echo "==> Ready! Isolated structure under ~/z_os"
ls -R ~/z_os
