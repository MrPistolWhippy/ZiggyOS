#include <stdint.h>

extern void init_global_descriptor_table(void);
extern void init_flash_page_mapper(void);
extern void init_video_dma_canvas(void);
extern void init_interrupt_priorities_sys(void);
extern int run_automated_tests_sys(void);
extern void app_radio_telemetry_init(void);
extern void app_stego_panic_init(void);
extern void app_stego_trigger_wipe(void);
extern void watchdog_kick(void);
extern void uart_putc(char c);
extern int uart_getc_nonblocking(char *out_char);
extern void print(const char *str);

void run_userland_shell(void) {
    char rx;
    print("\n--- ZiggyOS Interactive Userland Shell v1.0 ---\n");
    print("ziggy_user@deck:~$ ");

    while (1) {
        watchdog_kick();
        if (uart_getc_nonblocking(&rx)) {
            // Using literal ASCII integers to bypass terminal paste bugs (13=, 10=\n)
            if (rx == 13 || rx == 10) {
                print("\n[SHELL] Command executed. Processing return frame...\n");
                print("ziggy_user@deck:~$ ");
            } else if (rx == 'X' || rx == 'x') {
                app_stego_trigger_wipe();
            } else {
                uart_putc(rx);
            }
        }
    }
}

void kernel_main(void) {
    print("\n==================================================\n");
    print("[ZIGGYOS] BOOT: ARCHITECTURE SYNCHRONIZATION COMPLETE\n");
    print("==================================================\n");

    init_global_descriptor_table();
    init_flash_page_mapper();
    init_video_dma_canvas();
    init_interrupt_priorities_sys();

    if (!run_automated_tests_sys()) {
        app_stego_trigger_wipe();
    }

    app_radio_telemetry_init();
    app_stego_panic_init();

    run_userland_shell();
}
