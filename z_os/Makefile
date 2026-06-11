TARGET   = ziggyos
CC       = arm-none-eabi-gcc
LD       = arm-none-eabi-ld
SIZE     = arm-none-eabi-size
OBJCOPY  = arm-none-eabi-objcopy

CFLAGS   = -Wall -Wextra -O2 -ffreestanding -nostdlib -Isrc
LDFLAGS  = -T linker.ld

# Explicit pristine C files matching filesystem discovery map
SRCS_C   = src/main.c src/gdt.c src/interrupts.c src/kheap.c src/mouse.c src/pci.c src/tasks.c src/uart.c src/vfs.c src/vga.c src/archive.c src/mmu.c src/font.c

# Clean object dependencies tracking arrays
OBJS     = src/main.o src/archive.o src/gdt.o src/interrupts.o src/kheap.o src/mouse.o src/pci.o src/tasks.o src/uart.o src/vfs.o src/vga.o src/archive.o src/mmu.o src/font.o

.PHONY: all clean

all: $(TARGET).bin size

$(TARGET).bin: $(TARGET).elf
	$(OBJCOPY) -O binary $(TARGET).elf $(TARGET).bin

$(TARGET).elf: $(OBJS)
	$(LD) $(LDFLAGS) -o $(TARGET).elf $(OBJS)

src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

size: $(TARGET).elf
	$(SIZE) $(TARGET).elf

clean:
	rm -f src/*.o *.elf *.bin *.o
