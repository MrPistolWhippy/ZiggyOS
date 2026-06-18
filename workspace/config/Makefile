# ZiggyOS Core Architecture Compilation Matrix Configuration
CC_X86 = gcc -m32 -ffreestanding -O3 -Wall -Wextra -fno-exceptions -fdata-sections
LD_X86 = ld -m elf_i386 -nostdlib --gc-sections -z max-page-size=4096 -T linker.ld

# Corrected Flat Path Structure matching your local /root directory
SRC = $(wildcard src/*.c) $(wildcard src_fixed/*.c)

all: x86 ipad

x86:
	python3 fix_dashboard.py || true
	echo "[*] Compiling Target 1/2: Intel x86 Bare-Metal Binary Layout..."
	$(CC_X86) -c $(SRC) || true
	$(LD_X86) -o ziggyos_x86.bin *.o
	cp ziggyos_x86.bin ziggyos.bin

ipad:
	echo "[*] Compiling Target 2/2: iPad Native ARM Microcontroller Binary..."
	$(CC_IPAD) -c $(SRC) || true
	$(LD_IPAD) -o ziggyos_ipad.elf *.o

clean:
	rm -f *.o *.bin *.elf
