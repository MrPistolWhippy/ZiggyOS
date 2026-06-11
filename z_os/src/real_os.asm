# Standard 32-bit Multiboot 1 Header for GRUB (GNU Syntax)
.set MULTIBOOT_PAGE_ALIGN, 1 << 0
.set MULTIBOOT_MEMORY_INFO, 1 << 1
.set MULTIBOOT_HEADER_FLAGS, MULTIBOOT_PAGE_ALIGN | MULTIBOOT_MEMORY_INFO
.set MULTIBOOT_HEADER_MAGIC, 0x1BADB002
.set MULTIBOOT_CHECKSUM, -(MULTIBOOT_HEADER_MAGIC + MULTIBOOT_HEADER_FLAGS)

.section .multiboot
.align 4
.long MULTIBOOT_HEADER_MAGIC
.long MULTIBOOT_HEADER_FLAGS
.long MULTIBOOT_CHECKSUM

# Define our execution entry block
.section .text
.global _start
.type _start, @function

_start:
    # Disable hardware interrupts during system initialization
    cli

    # Call your C main entry function (kernel_main)
    # Ensure your function name in main.c matches this perfectly!
    call main

_infinite_halt_loop:
    hlt
    jmp _infinite_halt_loop

# Low-level routine to load our custom IDT pointer into the processor
.global idt_flush
idt_flush:
    movl 4(%esp), %eax    # Get the pointer address passed from the C function argument
    lidt (%eax)           # Load Interrupt Descriptor Table register instruction
    ret

# Generic Interrupt Service Routine Wrapper
.global isr_generic_wrapper
.extern common_exception_handler

isr_generic_wrapper:
    pushal                 # Push EDI, ESI, EBP, ESP, EBX, EDX, ECX, EAX
    cld                    # Clear direction flag for C code execution expectation
    call common_exception_handler
    popal                  # Restore general purpose registers
    iret                   # Specialized Intel Interrupt Return instruction

# Hardware Keyboard IRQ1 Wrapper
.global keyboard_handler_wrapper
.extern keyboard_handler

keyboard_handler_wrapper:
    pushal                 # Save all general-purpose registers
    cld                    # Clear direction flag for C compilation safety
    call keyboard_handler  # Call our C input processing function
    
    # Send End-Of-Interrupt (EOI) command byte (0x20) to the Master PIC (Port 0x20)
    # This tells the hardware it is safe to send the next keypress
    movb $0x20, %al
    outb %al, $0x20
    
    popal                  # Restore registers
    iret                   # Return from hardware interrupt

# Low-level routine to load our custom GDT structure and reload CPU segment mappings
.global gdt_flush
gdt_flush:
    movl 4(%esp), %eax    # Fetch the GDT pointer from the C function stack argument
    lgdt (%eax)           # Load Global Descriptor Table register instruction
    
    # Reload data segment registers to point to our new Kernel Data selector (0x10)
    movw $0x10, %ax
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %fs
    movw %ax, %gs
    movw %ax, %ss
    
    # Perform a far jump to flush the pre-fetch queue and load our Kernel Code selector (0x08)
    ljmp $0x08, $.reload_cs
.reload_cs:
    ret

# Hardware Timer IRQ0 Preemptive Scheduler Wrapper
.global timer_handler_wrapper
.extern schedule_preempt

timer_handler_wrapper:
    pushal                 # Push current task registers (EAX, ECX, EDX, EBX, ESP, EBP, ESI, EDI)
    cld

    movl %esp, %eax        # Pass the current task's Stack Pointer to the C scheduler
    pushl %eax
    call schedule_preempt
    movl %eax, %esp        # Switch the CPU stack pointer to the newly selected task's ESP!

    # Send End-Of-Interrupt (EOI) to the Master PIC (Port 0x20)
    movb $0x20, %al
    outb %al, $0x20

    popal                  # Pop the incoming task's registers
    iret                   # Return straight into the new task's execution line!

.global mouse_handler_wrapper
.extern mouse_handler
mouse_handler_wrapper:
    pushal
    cld
    call mouse_handler
    movb $0x20, %al
    outb %al, $0xA0
    outb %al, $0x20
    popal
    iret

# Low-level Keyboard IRQ1 Interrupt Wrapper
.global keyboard_handler_asm
.extern keyboard_callback
keyboard_handler_asm:
    pushal               # Push all current general-purpose task registers to the stack
    cld                  # Clear direction flags for C compliance
    call keyboard_callback # Jump straight into your C keyboard controller driver logic
    popal                # Restore all general-purpose registers cleanly
    iret                 # Return from Interrupt (restores CPU flags, CS, and EIP)
