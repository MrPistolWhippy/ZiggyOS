.section .text.boot
.global _start
_start:
    csrci mstatus, 8       /* Zero out machine-level interrupt flags */
    la sp, _stack_top      /* Map hardware stack pointer to physical RAM bounds */
la_clear_bss:
    la a0, __bss_start
    la a1, __bss_end
clear_loop:
    bge a0, a1, boot_main
    sw zero, 0(a0)
    addi a0, a0, 4
    j clear_loop
boot_main:
    jal ra, main_kernel_entry
halt_loop:
    wfi
    j halt_loop
