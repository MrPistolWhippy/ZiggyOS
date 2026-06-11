#include <stdint.h>

typedef struct {
    volatile uint32_t SR;   // Status Register
    volatile uint32_t DR;   // Data Register
} UART_t;

#define UART ((UART_t *)0x40004400)

char rb[256];
uint8_t wr = 0, rd = 0;

void uart_putc(char c) {
    rb[wr++] = c;
    if (UART->SR & 0x80) {
        UART->DR = rb[rd++];
    }
}
