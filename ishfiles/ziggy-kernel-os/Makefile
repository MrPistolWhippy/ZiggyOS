# --- Toolchain Configuration ---
CC      = arm-none-eabi-gcc
LD      = arm-none-eabi-ld
OBJCOPY = arm-none-eabi-objcopy
OBJDUMP = arm-none-eabi-objdump
SIZE    = arm-none-eabi-size

# --- Target and Flag Configuration ---
TARGET  = ziggyos
CFLAGS  = -mcpu=cortex-m4 -mthumb -O2 -Wall -nostdlib
LDFLAGS = -T linker.ld

# --- Source Layout Detection ---
SRCS    = src/vector_table.c src/main.c
OBJS    = $(SRCS:.c=.o)

.PHONY: all clean dump

all: $(TARGET).bin size

$(TARGET).elf: $(OBJS)
	$(LD) $(LDFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET).bin: $(TARGET).elf
	$(OBJCOPY) -O binary $< $@

size: $(TARGET).elf
	$(SIZE) $<

dump: $(TARGET).elf
	$(OBJDUMP) -d $< | head -n 30

clean:
	rm -f src/*.o *.elf *.bin
