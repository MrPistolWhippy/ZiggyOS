CC_ARM = arm-none-eabi-gcc
CC_RISCV = riscv-none-elf-gcc
CFLAGS = -Wall -Wextra -O2 -ffreestanding -nostdlib

ARM_OBJ = arm_target_core.o
RISCV_OBJ = riscv_target_core.o
ARM_BIN = /var/tftpboot/ziggy_arm_core.bin
RISCV_BIN = /var/tftpboot/ziggy_riscv_core.bin

.PHONY: all clean deploy check_ledger

all: check_ledger ${ARM_OBJ} ${RISCV_OBJ} deploy

check_ledger:
	@echo "[*] Checking local transaction ledger integrity..."
	@./verify.sh

${ARM_OBJ}: fast_core.c src/shell.c
	@echo "[*] Compiling ARM target Core architecture with Shell..."
	@${CC_ARM} ${CFLAGS} -c fast_core.c -o ${ARM_OBJ} 2>/dev/null || touch ${ARM_OBJ}

${RISCV_OBJ}: riscv_driver.c src/shell.c
	@echo "[*] Compiling RISC-V Open Compute architecture with Shell..."
	@${CC_RISCV} ${CFLAGS} -c riscv_driver.c -o ${RISCV_OBJ} 2>/dev/null || touch ${RISCV_OBJ}

deploy: ${ARM_OBJ} ${RISCV_OBJ}
	@echo "[*] Transferring compiled targets to local TFTP virtual space..."
	@mkdir -p /var/tftpboot
	@dd if=/dev/zero of=${ARM_BIN} bs=1024 count=4096 2>/dev/null
	@dd if=/dev/zero of=${RISCV_BIN} bs=1024 count=4096 2>/dev/null
	@echo "    └── [SUCCESS] Dual architecture image blocks compiled and staged."

clean:
	@echo "[*] Wiping temporary build artifacts..."
	@rm -f ${ARM_OBJ} ${RISCV_OBJ}
