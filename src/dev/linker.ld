MEMORY { FLASH(rx):ORIGIN=0,LEN=256K SRAM(rwx):ORIGIN=0x2,LEN=64K }
SECTIONS { .text : { *(.vectors) *(.text) } >FLASH }