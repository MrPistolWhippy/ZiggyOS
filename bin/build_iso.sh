#!/bin/bash
# ZiggyOS ISO Packaging Engine

echo "Syncing latest compiled binary artifact..."
cp src/os-kernel.bin isodir/boot/ 2>/dev/null || cp os-kernel.bin isodir/boot/

echo "Compiling bootable GRUB ISO image via xorriso backend..."
grub-mkrescue -o ziggyos.iso isodir

echo "Clean up deployment artifacts..."
rm -rf isodir

echo "=================================================="
echo " SUCCESS: ziggyos.iso is built and ready to boot! "
echo "=================================================="
