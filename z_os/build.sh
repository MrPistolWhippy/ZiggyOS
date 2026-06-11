#!/bin/sh
cd ~/z_os
gcc -m32 -c src/main.c -o main.o -std=gnu99 -ffreestanding -O2 && \
gcc -m32 -c src/uart.c -o uart.o -std=gnu99 -ffreestanding -O2 -Wno-unused-variable && \
gcc -m32 -c src/tasks.c -o tasks.o -std=gnu99 -ffreestanding -O2 && \
gcc -m32 -c src/interrupts.c -o interrupts.o -std=gnu99 -ffreestanding -O2 && \
gcc -m32 -c src/gdt.c -o gdt.o -std=gnu99 -ffreestanding -O2 && \
gcc -m32 -c src/kheap.c -o kheap.o -std=gnu99 -ffreestanding -O2 && \
gcc -m32 -c src/vfs.c -o vfs.o -std=gnu99 -ffreestanding -O2 && \
gcc -m32 -c src/pci.c -o pci.o -std=gnu99 -ffreestanding -O2 && \
gcc -m32 -c src/font.c -o font.o -std=gnu99 -ffreestanding -O2 && \
gcc -m32 -c src/mouse.c -o mouse.o -std=gnu99 -ffreestanding -O2 && \
gcc -m32 -c -x assembler-with-cpp src/real_os.asm -o boot.o && \
ld -m elf_i386 -T linker.ld -o ziggyos.bin boot.o main.o uart.o tasks.o interrupts.o gdt.o kheap.o vfs.o pci.o font.o mouse.o && \
strings ziggyos.bin | grep -E "Active|Stable|Enforced"
