#include <stdint.h>

void Reset_Handler(void) __attribute__((used));
void Reset_Handler(void) {
    extern void kernel_main(void);
    kernel_main();
}

void HardFault_Handler(void) __attribute__((used));
void HardFault_Handler(void) { while (1); }

void PendSV_Handler(void) __attribute__((used));
void PendSV_Handler(void) { while (1); }

void SysTick_Handler(void) __attribute__((used));
void SysTick_Handler(void) { while (1); }

void UART0_Handler(void) __attribute__((used));
void UART0_Handler(void) { while (1); }

void print(const char *str) __attribute__((used));
void print(const char *str) {
    extern void uart_putc(char c);
    while (*str) uart_putc(*str++);
}
