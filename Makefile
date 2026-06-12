# ZiggyOS Dual-Architecture Compilation Matrix Configuration
CC_X86 = gcc -m32 -ffreestanding -Os -Wall -Wextra -ffunction-sections -fdata-sections
LD_X86 = ld -m elf_i386 -nostdlib --gc-sections -z muldefs -T linker.ld

# iPad Native ARM compilation parameters
CC_IPAD = gcc -ffreestanding -Os -Wall -Wextra -ffunction-sections -fdata-sections
LD_IPAD = ld -nostdlib --gc-sections -z muldefs

SRC = src/main.c src/app_radio_telemetry.c src/app_stego_panic.c src/app_upgrade_layer.c src/app_userland.c src/extended_features.c src/extended_networks.c src/flash_driver.c src/matrix_5555.c src/mmu.c src/pci_semaphores.c src/uart_driver.c src/ultimate_55.c src/vector_table.c src/linker_fallbacks.c

all: x86 ipad

x86:
@echo "[*] Compiling Target 1/2: Intel x86 Bare-Metal Binary Layout..."
$(CC_X86) -c $(SRC)
mv *.o src/ 2>/dev/null || true
$(LD_X86) -o ziggyos_x86.bin src/*.o
cp ziggyos_x86.bin ziggyos.bin
cp ziggyos_x86.bin ~/z_os/iso_root/boot/ziggyos.bin

ipad:
@echo "[*] Compiling Target 2/2: iPad Native ARM Microcontroller Binary..."
$(CC_IPAD) -c $(SRC)
mv *.o src/ 2>/dev/null || true
$(LD_IPAD) -o ziggyos_ipad.elf src/*.o

clean:
rm -f src/*.o *.bin *.elf
