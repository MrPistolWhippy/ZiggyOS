#include <stdint.h>

void print(const char *str) {
    // Force direct 32-bit pointer writes to the emulator's custom MMIO port address
    volatile uint32_t *uart_dr = (volatile uint32_t *)0x40004404;
    while (*str) {
        *uart_dr = (uint32_t)(*str++);
    }
}
