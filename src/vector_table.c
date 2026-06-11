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
