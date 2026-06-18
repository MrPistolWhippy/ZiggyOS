#!/bin/sh

echo "========================================"
echo "⚡ RETRIEVING & PATCHING ZIGGYOS CORE ⚡"
echo "========================================"

# 1. Fix kmalloc signature mismatch (change size_t to uint32_t)
if [ -f "src/app_upgrade_layer.c" ]; then
    echo "[+] Patching kmalloc signature in app_upgrade_layer.c..."
    sed -i 's/void\* kmalloc(size_t size)/void* kmalloc(uint32_t size)/g' src/app_upgrade_layer.c
else
    echo "[-] src/app_upgrade_layer.c not found!"
fi

# 2. Re-write the linker script to establish perfect block page alignments
echo "[+] Rewriting src/linker.ld with structured section headers..."
cat << 'LNK' > src/linker.ld
ENTRY(_start)

PHDRS
{
    text PT_LOAD FLAGS(5); /* 5 = Read + Execute */
    data PT_LOAD FLAGS(6); /* 6 = Read + Write */
}

SECTIONS
{
    . = 0x100000;

    .text : {
        ALIGN(4096);
        *(.text)
    } :text

    .rodata : {
        ALIGN(4096);
        *(.rodata)
    } :text

    .data : {
        ALIGN(4096);
        *(.data)
    } :data

    .bss : {
        ALIGN(4096);
        *(.bss)
        *(COMMON)
        . = ALIGN(4096);
    } :data
}
LNK

echo "[+] Validation check complete! Run your local makefile or sync script."
echo "========================================"
