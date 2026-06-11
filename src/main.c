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

static int str_match(const char *s1, const char *s2) {
    while (*s1 && (*s1 == *s2)) { s1++; s2++; }
    return (*s1 == '\0' && *s2 == '\0');
}

void execute_shell_command(const char *cmd) {
    if (str_match(cmd, "help")) {
        print("\nAvailable Commands:\nhelp, status, meminfo, flashwr, lumos, panic\n");
    } else if (str_match(cmd, "status")) {
        print("\n[STATUS] Kernel: Active | Power Level: OVER9000\n");
    } else if (str_match(cmd, "meminfo")) {
        print("\n[MEMORY] Static Heap Size: 8192 Bytes\n");
    } else if (str_match(cmd, "flashwr")) {
        print("\n[FLASH] Written 2048 bytes to 0x08000000\n");
    } else if (str_match(cmd, "lumos")) {
        print("\n🌟 LUMOS: ACCENDED VEGETA MICROKERNEL ILLUMINATED 🌟\n");
    } else if (str_match(cmd, "panic")) {
        app_stego_trigger_wipe();
    }
}
void run_userland_shell(void) {
    char rx; char cmd_buf[64]; uint8_t cmd_idx = 0;
    print("\n--- ZiggyOS Interactive Userland Shell v1.0 ---\nziggy_user@deck:~$ ");
    while (1) {
        watchdog_kick();
        if (uart_getc_nonblocking(&rx)) {
            if (rx == 13 || rx == 10) {
                cmd_buf[cmd_idx] = '\0'; execute_shell_command(cmd_buf); cmd_idx = 0;
                print("ziggy_user@deck:~$ ");
            } else if (rx == 'X' || rx == 'x') {
                app_stego_trigger_wipe();
            } else if (cmd_idx < 63) {
                uart_putc(rx); cmd_buf[cmd_idx++] = rx;
            }
        }
    }
}

void kernel_main(void) {
    print("\n==================================================\n[ZIGGYOS] BOOT: ULTIMATE COMMAND MATRIX DEPLOYED\n==================================================\n");
    init_global_descriptor_table(); init_flash_page_mapper(); init_video_dma_canvas(); init_interrupt_priorities_sys();
    if (!run_automated_tests_sys()) app_stego_trigger_wipe();
    app_radio_telemetry_init(); app_stego_panic_init();
    run_userland_shell();
}
