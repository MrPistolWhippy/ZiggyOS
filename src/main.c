#include <stdint.h>

extern void app_radio_telemetry_init(void);
extern void app_radio_telemetry_tick(void);
extern void app_stego_panic_init(void);
extern void app_stego_trigger_wipe(void);
extern void uart_putc(char c);
extern int uart_getc_nonblocking(char *out_char);

void kernel_main(void) {
    char rx_cmd;
    char *welcome = "[KERNEL] ZiggyOS Initializing Boot Sequence...\n";
    while (*welcome) uart_putc(*welcome++);

    app_radio_telemetry_init();
    app_stego_panic_init();

    char *ready = "[KERNEL] Real-Time Polling Listener Running.\n";
    while (*ready) uart_putc(*ready++);

    while (1) {
        // Run background telemetry ticks
        app_radio_telemetry_tick();
        
        // Poll for incoming serial commands in real-time
        if (uart_getc_nonblocking(&rx_cmd)) {
            // Trigger emergency panic wipe sequence if 'X' or panic token received
            if (rx_cmd == 'X' || rx_cmd == 'x') {
                app_stego_trigger_wipe();
            } else {
                // Echo character confirmation back to connection channel
                uart_putc('[');
                uart_putc(rx_cmd);
                uart_putc(']');
            }
        }
    }
}
