MEMORY {
    FLASH (rx) : ORIGIN = 0x08000000, LENGTH = 512K
    SRAM (rwx) : ORIGIN = 0x20000000, LENGTH = 128M
}
SECTIONS {
    .text : { *(.text.boot) *(.text) } > FLASH
    .bss : { __bss_start = .; *(.bss) __bss_end = .; } > SRAM
    _stack_top = ORIGIN(SRAM) + LENGTH(SRAM);
}
