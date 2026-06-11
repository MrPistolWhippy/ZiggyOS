#!/bin/sh
set -e
cd ~/z_os

echo "==> Step 1: Cleaning and preparing directory tree..."
rm -rf iso_root
mkdir -p iso_root/boot/grub/i386-pc

echo "==> Step 2: Generating grub.cfg..."
cat << 'GRUBEOF' > iso_root/boot/grub/grub.cfg
set timeout=5
set default=0
menuentry "ZiggyOS (Bare Metal)" {
    multiboot /boot/ziggyos.bin
    boot
}
GRUBEOF

echo "==> Step 3: Compiling source files to 32-bit Freestanding targets..."
# Compile C files with warnings suppressed/handled cleanly
gcc -m32 -c src/main.c -o main.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra -Wno-implicit-function-declaration
gcc -m32 -c src/uart.c -o uart.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra -Wno-unused-variable

# FIXED: Explicitly forcing GCC to parse the .asm extension as an assembly file
gcc -m32 -c -x assembler-with-cpp src/real_os.asm -o boot.o

echo "==> Step 4: Linking final binary using linker.ld map..."
ld -m elf_i386 -T linker.ld -o iso_root/boot/ziggyos.bin boot.o main.o uart.o

echo "==> Step 5: Transferring local GRUB hardware modules..."
cp bootloader/i386-pc/*.mod iso_root/boot/grub/i386-pc/

echo "==> Step 6: Creating bootable ISO filesystem image..."
if command -v grub-mkrescue >/dev/null 2>&1; then
    grub-mkrescue -o ziggyos.iso iso_root
    echo "==> SUCCESS: ziggyos.iso has been successfully generated!"
elif command -v xorriso >/dev/null 2>&1; then
    xorriso -as mkisofs -R -b boot/grub/i386-pc/eltorito.img -no-emul-boot -boot-load-size 4 -boot-info-table -o ziggyos.iso iso_root
    echo "==> SUCCESS: ziggyos.iso built using raw xorriso fallback!"
else
    echo "==> WARNING: Binary compiled! To make an ISO install dependencies: apk add xorriso grub-bios"
fi
