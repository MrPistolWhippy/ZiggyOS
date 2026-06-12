ENTRY(_start)

PHDRS
{
    text PT_LOAD FLAGS(5); /* 5 = Read + Execute */
    data PT_LOAD FLAGS(6); /* 6 = Read + Write */
}

SECTIONS
{
    . = 0x100000;
    MEMORY { RAM (rwx) : ORIGIN = 0x100000, LENGTH = 4M }

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
