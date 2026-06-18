MEMORY
{
    FLASH (rx) : ORIGIN = 0x08000000, LENGTH = 4M
    RAM  (rwx) : ORIGIN = 0x20000000, LENGTH = 1M
}

SECTIONS
{
    .vectors :
    {
        KEEP(*(.vectors))
    } > FLASH

    .text :
    {
        *(.text*)
        *(.rodata*)
    } > FLASH

    .data :
    {
        *(.data*)
    } > RAM AT > FLASH

    .bss :
    {
        *(.bss*)
    } > RAM
}

ASSERT(SIZEOF(.vectors) + SIZEOF(.text) <= LENGTH(FLASH), "ERROR: FLASH WINDOW STORAGE SPACE OVERFLOW!")
