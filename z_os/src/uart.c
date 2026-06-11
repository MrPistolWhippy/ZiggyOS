#include <stdint.h>

// Standard ARM MMIO UART Base Address mapping placeholder
// (e.g., QEMU VersatilePB UART0 base is usually 0x101f1000)
#define UART0_BASE 0x101f1000
#define UART_DR    ((volatile uint32_t*)(UART0_BASE + 0x00))
#define UART_FR    ((volatile uint32_t*)(UART0_BASE + 0x18))

uint8_t inb(uint16_t port) {
    (void)port;
    return 0;
}

void outb(uint16_t port, uint8_t data) {
    (void)port;
    (void)data;
}

int is_transmit_empty(void) {
    // Return 1 so your printing functions don't get stuck in an infinite loop spin
    return 1; 
}

void write_serial(char a) {
    // True ARM MMIO output sequence
    *UART_DR = (uint32_t)a;
}

void init_serial(void) {
    // ARM initialization stub
}
