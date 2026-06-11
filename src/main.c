#include <stdint.h>

// Submodule function declarations
extern void app_radio_telemetry_init(void);
extern void app_radio_telemetry_tick(void);
extern void app_stego_panic_init(void);
extern void app_stego_trigger_wipe(void);
extern void uart_putc(char c);

void kernel_main(void) {
    char *welcome = "[KERNEL] ZiggyOS Initializing Boot Sequence...\n";
    while (*welcome) {
        uart_putc(*welcome++);
    }

    // Initialize both application subsystems
    app_radio_telemetry_init();
    app_stego_panic_init();

    char *ready = "[KERNEL] Entering background system task loops.\n";
    while (*ready) {
        uart_putc(*ready++);
    }

    // Main operational execution thread
    while (1) {
        // Continuously tick background telemetry radio broadcasts
        app_radio_telemetry_tick();
        
        // Example: Simulated physical safety interrupt check
        // If a safety pin layout breaks, trip the wipe line immediately
        // if (hardware_panic_pin_tripped()) { app_stego_trigger_wipe(); }
    }
}
