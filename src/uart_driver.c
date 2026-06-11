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

// Non-blocking real-time polling listener routine to read incoming telemetry commands
int uart_getc_nonblocking(char *out_char) {
    // Check if the Receiver Data Register Full (RXNE) status bit is high
    if (UART->SR & 0x20) { 
        *out_char = (char)(UART->DR & 0xFF);
        return 1; // Return true indicating data was fetched successfully
    }
    return 0; // Return false immediately if no data is present in the buffer
}
