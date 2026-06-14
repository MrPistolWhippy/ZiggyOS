.global _start
.intel_syntax noprefix
_start:
    cli
    mov eax, 0x10
    mov ds, ax
    mov esp, 0x90000
    hlt
