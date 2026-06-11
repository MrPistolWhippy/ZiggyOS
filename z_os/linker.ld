ENTRY(_start)
SECTIONS
{
    . = 1M;

    .text : ALIGN(4K)
    {
        *(.multiboot)
        *(.text)
    }

    .rodata : ALIGN(4K) { *(.rodata) }
    .data   : ALIGN(4K) { *(.data) }
    .bss    : ALIGN(4K) { *(.bss)  *(COMMON) }

    /* The end symbol marks the exact final byte of the kernel binary */
    _kernel_end = .;
}
