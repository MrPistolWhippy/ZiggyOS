#include <stdint.h>

// Vector Table System Exception Handlers
void Reset_Handler(void) {
    extern void kernel_main(void);
    kernel_main();
}

void Default_Handler(void) {
    while (1);
}

void HardFault_Handler(void) __attribute__((weak, alias("Default_Handler")));
void PendSV_Handler(void)    __attribute__((weak, alias("Default_Handler")));
void SysTick_Handler(void)   __attribute__((weak, alias("Default_Handler")));
void UART0_Handler(void)     __attribute__((weak, alias("Default_Handler")));

// System-wide print utility string loop
void print(const char *str) {
    extern void uart_putc(char c);
    while (*str) {
        uart_putc(*str++);
    }
}
