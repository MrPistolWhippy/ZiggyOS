CC = gcc
LD = ld
CFLAGS = -ffreestanding -Os -Wall -Wextra -ffunction-sections -fdata-sections
LDFLAGS = -nostdlib --no-warn-rwx-segments --gc-sections -T src/linker.ld

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
