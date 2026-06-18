.global _start

.section .text
_start:
    call kernel_main

hang:
    cli
    hlt
    jmp hang
