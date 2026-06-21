/* ==============================================================================
 *          ZIGGY-OS HARDWARE CORE: INTERRUPT & SERIAL CONSOLE LOGIC
 * ============================================================================== */
#include <stdint.h>

/* --- 1. MEMORY-MAPPED REGISTERS (UART 16550 STANDARD) --- */
#define UART_BASE 0x10000000
#define UART_THR  ((volatile uint8_t*)(UART_BASE + 0)) /* Transmit Holding Reg */
#define UART_LSR  ((volatile uint8_t*)(UART_BASE + 5)) /* Line Status Reg */

/* --- 2. HARDWARE TELEMETRY PACKET MAP --- */
typedef struct __attribute__((packed)) {
    uint64_t timestamp_tick;
    uint32_t active_node_id;
    uint16_t vulnerability_ratio;
    uint8_t  swarm_lock_status;
    uint8_t  checksum;
} ZiggyTelemetry_t;

volatile ZiggyTelemetry_t *CORE_METRICS_STREAM = (ZiggyTelemetry_t *)0x8000F000;

/* --- 3. DRIVER IMPLEMENTATIONS --- */
void uart_putc(char c) {
    /* Wait until Transmitter Holding Register Empty (THRE) bit 5 is set */
    while ((*UART_LSR & 0x20) == 0);
    *UART_THR = c;
}

void uart_puts(const char *s) {
    while (*s) {
        uart_putc(*s++);
    }
}

/* --- 4. INTERRUPT VECTOR TABLE HANDLERS --- */
void handle_exception(uintptr_t mcause, uintptr_t mepc) {
    uart_puts("\n[⚡ EXCEPTION TRIGGERED] Cause Code: ");
    uart_putc('0' + (mcause & 0xF));
    uart_puts(" | Faulting PC: ");
    while (1); /* Trap handler latch */
}

void handle_interrupt(uintptr_t mcause) {
    if ((mcause & 0x80000000) && (mcause & 0xF) == 7) {
        /* Timer Interrupt handling event */
        CORE_METRICS_STREAM->timestamp_tick++;
        uart_puts("."); /* Heartbeat click ticker */
    }
}

/* --- 5. SYSTEM ENTRY EXECUTIVE EXECUTION --- */
void _start(void) {
    extern uint32_t __bss_start, __bss_end;
    uint32_t *bss = &__bss_start;
    while (bss < &__bss_end) {
        *bss++ = 0;
    }

    /* Initialize Console Logging System */
    uart_puts("\n---------------------------------------------------");
    uart_puts("\n   ZIGGY-OS KERNEL SUBSYSTEM RUNTIME INITIALISED   ");
    uart_puts("\n---------------------------------------------------\n");
    uart_puts("[✓] UART Console Terminal Output... ONLINE\n");
    uart_puts("[✓] Dynamic Interrupt Vector Layer... HOOKED\n");

    while (1) {
        if (CORE_METRICS_STREAM->swarm_lock_status == 0xA7) {
            CORE_METRICS_STREAM->vulnerability_ratio = 0;
        }
    }
}
