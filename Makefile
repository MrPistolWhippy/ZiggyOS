CC = gcc
CFLAGS = -m32 -ffreestanding -O2 -Wall -Wextra
LDFLAGS = -m32 -nostdlib -Xlinker --no-warn-rwx-segments -T src/linker.ld

SRC = $(wildcard src/*.c)
OBJ = $(SRC:.c=.o)
TARGET = os-kernel.bin

all: $(TARGET)

$(TARGET): $(OBJ)
$(CC) $(LDFLAGS) -o $@ $^ || ld -m elf_i386 -T src/linker.ld -o $@ $^

%.o: %.c
$(CC) $(CFLAGS) -c $< -o $@

clean:
rm -f src/*.o $(TARGET)
