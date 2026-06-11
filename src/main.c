#include <stdint.h>

extern void app_radio_telemetry_init(void);
extern void app_radio_telemetry_tick(void);
extern void app_stego_panic_init(void);
extern void app_stego_trigger_wipe(void);
extern void uart_putc(char c);
extern int uart_getc_nonblocking(char *out_char);

// New component linkages
extern void init_interrupt_priorities(void);
extern int run_automated_tests(void);
extern void print(const char *str);

void kernel_main(void) {
    char rx_cmd;
    print("[KERNEL] ZiggyOS Extended Architecture Booting...\n");

    // Initialize priorities, run self-tests, and start driver stacks
    init_interrupt_priorities();
    if (!run_automated_tests()) {
        app_stego_trigger_wipe(); // Panic if firmware validation fails
    }

    app_radio_telemetry_init();
    app_stego_panic_init();
    print("[KERNEL] All 5 submodules online. Listening for telemetry commands.\n");

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
