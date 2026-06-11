#include <stdint.h>

void print(const char *str) {
    extern void uart_putc(char c);
    while (*str) uart_putc(*str++);
}
