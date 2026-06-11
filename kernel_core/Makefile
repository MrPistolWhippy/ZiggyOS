CC = gcc
AS = as --32
CFLAGS = -m32 -ffreestanding -O2 -Wall -Wextra

OBJ = src/boot.o src/main.o src/pci.o src/shell.o src/keyboard.o src/keymap.o src/mmu.o src/app.o

all: os-kernel.bin

src/boot.o: src/boot.s
	$(AS) $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

os-kernel.bin: $(OBJ)
	ld -m elf_i386 -T linker.ld -o os-kernel.bin $(OBJ)

clean:
	rm -f src/*.o *.bin *.o
