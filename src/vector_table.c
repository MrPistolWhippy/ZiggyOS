#include <stdint.h>
void Reset_Handler(void); void SysTick_Handler(void); void HardFault_Handler(void);
extern void PendSV_Handler(void); extern void UART0_Handler(void);
__attribute__((section(".vectors")))
void (* const vector_table[])(void) = {
    (void (*)(void))0x10018000, Reset_Handler, 0, HardFault_Handler,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, PendSV_Handler, 0, SysTick_Handler,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, UART0_Handler
};

// --- SELF-CONTAINED INTERRUPT EXCEPTION FALLBACK HANDLERS ---
void Reset_Handler(void) {
    extern void kernel_main(void);
    kernel_main();
}

void Default_Handler(void) {
    while (1);
}

void HardFault_Handler(void) { Default_Handler(); }
void PendSV_Handler(void)    { Default_Handler(); }
void SysTick_Handler(void)   { Default_Handler(); }
void UART0_Handler(void)     { Default_Handler(); }

// Master system execution clock tick tracker
volatile uint64_t system_jiffies_ticks = 0;

// High-speed low-level hardware interrupt service wrapper for IRQ 0 (Timer)
void handle_pit_timer_interrupt_tick(void) {
    system_jiffies_ticks++;
    
    // Periodically print clock diagnostics to show active multi-threading scheduling pools
    if (system_jiffies_ticks % 100 == 0) {
        print("[CLOCK] 100 hardware ticks registered. Scheduling pool active.n");
    }
    
    // Send End-of-Interrupt (EOI) command signal byte back to Master PIC (Port 0x20)
    // This acknowledges the signal and tells the hardware it is safe to issue the next tick
    __asm__ volatile ("outb %0, %1" : : "a"((uint8_t)0x20), "Nd"((uint16_t)0x20));
}
