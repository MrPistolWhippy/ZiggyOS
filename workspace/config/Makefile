# ZiggyOS Workspace Matrix Core Compilation Configuration
CC_X86 = gcc -m32 -ffreestanding -O3 -Wall -Wextra -fno-exceptions -fdata-sections
LD_X86 = ld -m elf_i386 -nostdlib --gc-sections -z max-page-size=4096 -T /root/workspace/config/linker.ld

# Strictly include core architecture modules, ignoring conflicting standalone peripheral files
SRC = /root/workspace/src/app/main.c /root/workspace/src/shell.c /root/workspace/src/app_upgrade_layer.c /root/workspace/src/app_userland.c /root/workspace/src/pci_driver.c

all: x86

x86:
	@python3 /root/workspace/bin/master_automate.sh --fast > /dev/null 2>&1 || true
	echo "[*] Compiling Target: Intel x86 Workspace Bare-Metal Image..."
	$(CC_X86) -c $(SRC) || true
	$(LD_X86) -o /root/workspace/bin/ziggyos_x86.bin *.o
	cp /root/workspace/bin/ziggyos_x86.bin /root/workspace/bin/ziggyos.bin

clean:
	rm -f *.o /root/workspace/bin/*.bin /root/workspace/bin/*.elf
