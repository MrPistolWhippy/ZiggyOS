extern int shell_strcmp(const char *s1, const char *s2);
extern void shell_print(const char *str);
static int cursor_pos = 0;

static inline void outb(unsigned short port, unsigned char val) {
    __asm__ volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

void shell_print_color(const char *str, unsigned char color) {
    volatile unsigned short *video_memory = (volatile unsigned short *)0xB8000;
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] == '\n') {
            cursor_pos = ((cursor_pos / 80) + 1) * 80;
        } else {
            video_memory[cursor_pos] = (color << 8) | str[i];
            cursor_pos++;
        }
        if (cursor_pos >= 2000) { cursor_pos = 24 * 80; }
    }
}

void shell_execute_command(const char *shell_buffer) {
    if (shell_strcmp(shell_buffer, "help") == 0) {
        shell_print("Commands: help, uptime, reboot, kamehamehaaaaa\n");
    } else if (shell_strcmp(shell_buffer, "uptime") == 0) {
        shell_print("Uptime: 00 Hours, 01 Mins\n");
    } else if (shell_strcmp(shell_buffer, "kamehamehaaaaa") == 0) {
        shell_print_color("HAAAAA!!!!\n", 0x09);
    } else if (shell_strcmp(shell_buffer, "reboot") == 0) {
        shell_print("Resetting hardware CPU...\n");
        outb(0x64, 0xFE);
    } else {
        shell_print("Unknown.\n");
    }
}
