# ==============================================================================
#          ZIGGY-OS LOW-LEVEL BOOTSTRAP: HARDWARE INITIALIZATION HARNESS
# ==============================================================================
.section .text._start
.global _start

_start:
    # 1. Disable all interrupts globally during early boot phase
    csrw mstatus, zero

    # 2. Set up the stack pointer (growing downwards from top of SRAM space)
    # Target address points to high RAM boundary relative to your linker origin
    la sp, _stack_top

    # 3. Clear core temporary and argument integer registers
    li ra, 0
    li a0, 0
    li a1, 0
    li a2, 0

    # 4. Jump safely into the initialized C runtime matrix subroutine 
    tail _start
