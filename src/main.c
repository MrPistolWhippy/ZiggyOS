#include <stdint.h>

extern void app_radio_telemetry_init(void);
extern void app_radio_telemetry_tick(void);
extern void app_stego_panic_init(void);
extern void app_stego_trigger_wipe(void);
extern void uart_putc(char c);
extern int uart_getc_nonblocking(char *out_char);

// Synchronized system extensions
extern void init_interrupt_priorities_sys(void);
extern int run_automated_tests_sys(void);

void kernel_main(void) {
    char rx_cmd;
    init_interrupt_priorities_sys();
    
    if (!run_automated_tests_sys()) {
        app_stego_trigger_wipe();
    }

    app_radio_telemetry_init();
    app_stego_panic_init();

    while (1) {
        app_radio_telemetry_tick();
        if (uart_getc_nonblocking(&rx_cmd)) {
            if (rx_cmd == 'X' || rx_cmd == 'x') {
                app_stego_trigger_wipe();
            } else {
                uart_putc('[');
                uart_putc(rx_cmd);
                uart_putc(']');
            }
        }
    }
}
