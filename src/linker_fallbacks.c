#include <stdint.h>

// Missing UART terminal and output drivers
void uart_putc(char c) { (void)c; }
char uart_getc_nonblocking(void) { return 0; }
void print(const char* str) { (void)str; }

// Missing system core functions 
void probe_pci_bus_sys(void) {}
void irq_register_shared_handler_sys(int irq, void (*handler)(void*), void* arg) { (void)irq; (void)handler; (void)arg; }
void profile_task_runtime_sys(void) {}
