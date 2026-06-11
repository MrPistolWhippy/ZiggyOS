#include <stdint.h>

extern void init_global_descriptor_table(void);
extern void jump_to_user_mode(void (*user_func)(void));
extern void watchdog_kick(void);
extern void uart_putc(char c);
extern int uart_getc_nonblocking(char *out_char);
extern void print(const char *str);
extern void app_stego_trigger_wipe(void);

static int str_match(const char *s1, const char *s2) {
    while (*s1 && (*s1 == *s2)) { s1++; s2++; }
    return (*s1 == '\0' && *s2 == '\0');
}

void execute_shell_command(const char *cmd) {
    if (str_match(cmd, "help")) {
        print("\nAvailable Rings Commands:\nhelp, status, panic\n");
    } else if (str_match(cmd, "status")) {
        print("\n[STATUS] Privilege Level: Ring 3 (Userland Isolation Mode)\n");
    } else if (str_match(cmd, "panic")) {
        app_stego_trigger_wipe();
    }
}

void run_userland_shell(void) {
    char rx; char cmd_buf[64]; uint8_t cmd_idx = 0;
    print("\n--- ZiggyOS Privilege Ring 3 Shell Active ---\nziggy_user@deck:~$ ");
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
    print("\n==================================================\n");
    print("[ZIGGYOS] BOOT: PREPARING PRIVILEGE RING SEPARATION\n");
    print("==================================================\n");

    init_global_descriptor_table();
    jump_to_user_mode(run_userland_shell);
}
