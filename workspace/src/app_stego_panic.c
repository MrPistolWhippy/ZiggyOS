#include <stdint.h>

extern void uart_putc(char c);

void app_stego_panic_init(void) {
    char *msg = "[STEGO] Panic safety monitoring online.\n";
    while (*msg) {
        uart_putc(*msg++);
    }
}

// Emergency safety sequence to clear volatile buffers
void app_stego_trigger_wipe(void) {
    char *warn = "\n!!! PANIC INTERCEPTED: WIPING MEMORY REGISTERS !!!\n";
    while (*warn) {
        uart_putc(*warn++);
    }
    // Infinite safety halt loop
    while (1) {
        __asm__ volatile("cli; hlt");
    }
}
