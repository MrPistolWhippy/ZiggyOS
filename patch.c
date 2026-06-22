// Custom minimal bare-metal print stub for compilation resolution
void print(const char *str) {
    // In actual hardware, you would loop and write to a UART TX register here
    while (*str) {
        volatile char *uart_tx = (volatile char *)0x10000000; // Example MMIO UART address
        *uart_tx = *str++;
    }
}
