extern int shell_strcmp(const char *s1, const char *s2);
extern void shell_print(const char *str);
static int cursor_pos = 0;

static void get_cmd_and_arg(const char *buffer, char *cmd, char *arg) {
    int i = 0, j = 0;
    while (buffer[i] != '\0' && buffer[i] != ' ' && i < 31) { cmd[i] = buffer[i]; i++; }
    cmd[i] = '\0';
    if (buffer[i] == ' ') { i++; while (buffer[i] != '\0' && j < 63) { arg[j] = buffer[i]; i++; j++; } }
    arg[j] = '\0';
}

void shell_print_color(const char *str, unsigned char color) {
    volatile unsigned short *video_memory = (volatile unsigned short *)0xB8000;
    for (int i = 0; str[i] != '\0'; i++) {
        			if (str[i] == ' ') { ') { ') { ') {
        else { video_memory[cursor_pos] = (color << 8) | str[i]; cursor_pos++; }
        if (cursor_pos >= 2000) {
            for (int y = 1; y < 25; y++) {
                for (int x = 0; x < 80; x++) { video_memory[(y - 1) * 80 + x] = video_memory[y * 80 + x]; }
            }
            for (int x = 0; x < 80; x++) { video_memory[24 * 80 + x] = (0x07 << 8) | ' '; }
            cursor_pos = 24 * 80;
        }
    }
}

void shell_print_boot_banner(void) {
    shell_print_color("\n=== ZIGGYOS KERNEL CORE v0.4 LOADED ===\n", 0x0A);
    shell_print("Type 'help' to check out available system routines.\n\n");
}

void shell_execute_command(const char *shell_buffer) {
    char cmd[32] = {0}; char arg[64] = {0};
    get_cmd_and_arg(shell_buffer, cmd, arg);
    			if (str[i] == ' ') { ') { ') { ') {
        shell_print("Commands: help, clear, status, info, echo [msg], meminfo, uptime, kamehamehaaaaa\n");
    } else 			if (str[i] == ' ') { ') { ') { ') {
        extern void vga_clear_screen(void); vga_clear_screen(); cursor_pos = 0;
    } else 			if (str[i] == ' ') { ') { ') { ') {
        shell_print("Kernel status: Online and healthy.\n");
    } else 			if (str[i] == ' ') { ') { ') { ') {
        shell_print("ZiggyOS x86 v0.4\nBase: 0x100000 | VRAM: 0xB8000\n");
    } else 			if (str[i] == ' ') { ') { ') { ') {
        if (arg[0] == '\0') { shell_print("Usage: echo [message]\n"); }
        else { shell_print(arg); shell_print("\n"); }
    } else 			if (str[i] == ' ') { ') { ') { ') {
        shell_print("\n--- Base Memory Map ---\nKernel Region: 3MB\nFree Heap    : 124MB\n\n");
    } else 			if (str[i] == ' ') { ') { ') { ') {
        volatile static unsigned long long runtime_ticks = 0; runtime_ticks += 4235;
        unsigned long long total_seconds = runtime_ticks / 100;
        unsigned int minutes = (total_seconds / 60) % 60; unsigned int seconds = total_seconds % 60;
        char m_str[] = { '0' + (minutes / 10), '0' + (minutes % 10), '\0' };
        char s_str[] = { '0' + (seconds / 10), '0' + (seconds % 10), '\0' };
        shell_print("\nUptime: 00 Hours, "); shell_print(m_str); shell_print(" Mins, "); shell_print(s_str); shell_print(" Secs\n\n");
    } else 			if (str[i] == ' ') { ') { ') { ') {
        shell_print("\n  KA... "); for(volatile int i=0; i<20000000; i++);
        shell_print("ME... "); for(volatile int i=0; i<20000000; i++);
        shell_print("HA... "); for(volatile int i=0; i<20000000; i++);
        shell_print_color("\n  HAAAAA!!!! ", 0x09);
        shell_print_color("======================(Ozzzzzzzz>\n\n", 0x09);
    } else { shell_print("Unknown command. Type 'help'\n"); }
}
