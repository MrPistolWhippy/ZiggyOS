ENTRY(_start)

MEMORY {
    RAM (rwx) : ORIGIN = 0x80000000, LENGTH = 128M
}

SECTIONS {
    . = ORIGIN(RAM);
    
    .text : {
        *(.text._start)
        *(.text .text.*)
    } > RAM

    .data : { *(.data .data.*) } > RAM
    .bss : {
        __bss_start = .;
        *(.bss .bss.*)
        *(COMMON)
        __bss_end = .;
    } > RAM

    /* Set stack top at the exact end limit of the 128MB RAM boundary */
    _stack_top = ORIGIN(RAM) + LENGTH(RAM);

    /DISCARD/ : {
        *(.comment)
        *(.eh_frame)
    }
}
