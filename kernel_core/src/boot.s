.set MAGIC, 0x1BADB002
.set FLAGS, 0x07
.set CHECKSUM, -(MAGIC + FLAGS)

.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

.section .text
.global _start
.type _start, @function
_start:
	mov $stack_top, %esp
	call kernel_main
	cli
1:	hlt
	jmp 1b

.section .bss
.align 16
stack_bottom:
.skip 16384
stack_top:

.global irq_timer_handler
.extern schedule_preempt
irq_timer_handler:
	pusha
	push %ds
	push %es
	mov $0x10, %ax
	mov %ax, %ds
	mov %ax, %es
	push %esp
	call schedule_preempt
	mov %eax, %esp
	pop %es
	pop %ds
	popa
	mov $0x20, %al
	out %al, $0x20
	iret
