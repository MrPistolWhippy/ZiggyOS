CC = gcc
LD = ld
CFLAGS = -m32 -ffreestanding -Os -Wall -Wextra -ffunction-sections -fdata-sections
LDFLAGS = -m elf_i386 -nostdlib --gc-sections -T linker.ld

SRC = $(wildcard src/*.c)
OBJ = $(SRC:.c=.o)
TARGET = os-kernel.bin

all: $(TARGET)

$(TARGET): $(OBJ)
	$(LD) $(LDFLAGS) -o $(TARGET) $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f src/*.o $(TARGET)
