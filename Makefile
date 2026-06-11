CC = gcc
CFLAGS = -m32 -ffreestanding -O2 -Wall -Wextra
LDFLAGS = -m32 -nostdlib -Xlinker --no-warn-rwx-segments -T src/linker.ld

SRC = $(wildcard src/*.c)
OBJ = $(SRC:.c=.o)
TARGET = os-kernel.bin

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) -m32 -ffreestanding -nostdlib -Xlinker --no-warn-rwx-segments -T src/linker.ld -o $@ $^ || ld -m elf_i386 -T src/linker.ld -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f src/*.o $(TARGET)
	$(CC) $(CFLAGS) -c src/app_userland.c -o src/app_userland.o
