CC = gcc
CFLAGS = -m32 -ffreestanding -O2 -Wall -Wextra
OBJ = src/main.o src/pci.o src/shell.o src/keyboard.o src/keymap.o src/mmu.o src/app.o src/app_radio_telemetry.o

all: os-kernel.bin

%.o: %.c
	  -c $< -o 

os-kernel.bin: 
	ld -m elf_i386 -T linker.ld -o os-kernel.bin 

clean:
	rm -f src/*.o *.bin *.o
