#include <stdint.h>

// Core symbol overrides to satisfy tracking parameters
void _exit(int status) {
    (void)status;
    while (1);
}

void __stack_chk_fail(void) {
    while (1);
}

// System Exception and Interrupt Fallback Handlers
void Reset_Handler(void) {
    extern void kernel_main(void);
    kernel_main();
}

void Default_Handler(void) {
    while (1);
}

// Map vector requirements to the default infinite-loop handler
void HardFault_Handler(void) __attribute__((weak, alias("Default_Handler")));
void PendSV_Handler(void)    __attribute__((weak, alias("Default_Handler")));
void SysTick_Handler(void)   __attribute__((weak, alias("Default_Handler")));
void UART0_Handler(void)     __attribute__((weak, alias("Default_Handler")));
