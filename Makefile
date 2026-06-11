CC = gcc
CFLAGS = -m32 -ffreestanding -Os -Wall -Wextra -ffunction-sections -fdata-sections
LDFLAGS = -m32 -nostdlib -Xlinker --no-warn-rwx-segments -Xlinker --gc-sections -T src/linker.ld

SRC = $(wildcard src/*.c)
OBJ = $(SRC:.c=.o)
TARGET = os-kernel.bin

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(LDFLAGS) -o $(TARGET) $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f src/*.o $(TARGET)
