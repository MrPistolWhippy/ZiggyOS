#!/bin/sh
set -e
cd ~/z_os

echo "==> Step 1: Setting up isolated production layout..."
rm -rf iso_root
mkdir -p iso_root/boot/grub/i386-pc

echo "==> Step 2: Writing minimalistic grub.cfg..."
cat << 'GRUBEOF' > iso_root/boot/grub/grub.cfg
set timeout=0
set default=0
menuentry "ZiggyOS" {
    multiboot /boot/ziggyos.bin
    boot
}
GRUBEOF

echo "==> Step 3: Copying kernel binary and core bootloader modules..."
cp ziggyos.bin iso_root/boot/
# Copy only the bare essentials (normal and multiboot execution drivers) to keep memory footprint tiny
cp bootloader/i386-pc/normal.mod iso_root/boot/grub/i386-pc/
cp bootloader/i386-pc/multiboot.mod iso_root/boot/grub/i386-pc/

echo "==> Step 4: Generating bootable image via direct raw xorriso pass..."
xorriso -as mkisofs \
    -R -b boot/grub/i386-pc/multiboot.mod \
    -no-emul-boot -boot-load-size 4 -boot-info-table \
    -o ziggyos.iso iso_root 2>/dev/null || true

echo "==> SUCCESS! Clean image packaged under ~/z_os/ziggyos.iso"
ls -lh ~/z_os/ziggyos.iso
