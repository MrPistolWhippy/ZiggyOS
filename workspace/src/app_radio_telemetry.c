#include <stdint.h>

extern void uart_putc(char c);
// Reference the authentic master hardware execution clock ticks from vector_table.c
extern volatile uint64_t system_jiffies_ticks;

void app_radio_telemetry_init(void) {
    char *init_msg = "[RADIO] Initializing Telemetry Stream...\n";
    while (*init_msg) {
        uart_putc(*init_msg++);
    }
}

void app_radio_telemetry_tick(void) {
    static uint64_t last_sync_tick = 0;
    
    // Check if 100 hardware clock ticks (1 full second) have elapsed cleanly
    if (system_jiffies_ticks - last_sync_tick >= 100) {
        last_sync_tick = system_jiffies_ticks;
        uart_putc('~'); // Output a wave character indicating an active radio telemetry broadcast beacon
    }
}
