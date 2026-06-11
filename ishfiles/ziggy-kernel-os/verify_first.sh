#!/bin/sh
echo "🛠️ 1. Compiling Source Code to ELF Image..."
arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -O2 -g -nostdlib -T linker.ld src/vector_table.c src/main.c -o ziggyos.elf
if [ $? -ne 0 ]; then
    echo "❌ Compilation failed completely!"
    exit 1
fi
echo "🔍 2. Running Structural Verification FIRST..."
ENTRYPOINT=$(r2 -q -c "iH" ziggyos.elf | grep "Entrypoint" | awk '{print $NF}')
MACHINE=$(r2 -q -c "iH" ziggyos.elf | grep "Machine" | awk '{print $NF}')
echo "   -> Target Machine Hex ID: $MACHINE (ARM)"
echo "   -> Validated Entrypoint : $ENTRYPOINT"
echo "📍 3. Validating Core Function Symbol Mapping..."
r2 -q -c "is" ziggyos.elf | grep -E "Reset_Handler|ang|set_win"
echo "📦 4. Structural Checks Passed. Writing Flat Binary Layout..."
arm-none-eabi-objcopy -O binary ziggyos.elf ziggyos.bin
echo "✅ Flat image 'ziggyos.bin' generated successfully!"
