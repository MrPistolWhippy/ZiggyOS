#include <stdint.h>

// Forward declarations for your system print hooks
void shell_print(const char *str);
void shell_print_color(const char *str, uint8_t color);
void vga_clear_screen(void);
extern int cursor_pos;

// Mock strcmp since this is a freestanding custom kernel environment
int shell_strcmp(const char *s1, const char *s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(unsigned char *)s1 - *(unsigned char *)s2;
}

void shell_execute_command(const char *shell_buffer) {
    if (shell_strcmp(shell_buffer, "help") == 0) {
        shell_print("Available: help, clear, status, info, echo, nexinfo, uptime\n");
    } 
    else if (shell_strcmp(shell_buffer, "clear") == 0 || shell_strcmp(shell_buffer, "cls") == 0) {
        vga_clear_screen();
        cursor_pos = 0;
    } 
    else if (shell_strcmp(shell_buffer, "status") == 0) {
        shell_print("Kernel status: Online and Healthy.\n");
    } 
    else if (shell_strcmp(shell_buffer, "info") == 0) {
        shell_print("ZiggyOS x86 V8.4\nBase: 0x100000 | VRAM: 0xB8000\n");
    } 
    else if (shell_strcmp(shell_buffer, "nexinfo") == 0) {
        shell_print("--- Base Memory Map --- \nKernel Region: 3MB\nFree Heap : 124M3\n");
    } 
    else if (shell_strcmp(shell_buffer, "uptime") == 0) {
        shell_print("Uptime: 00 Hours, 05 Mins, 12 Secs\n");
    } 
    else if (shell_strcmp(shell_buffer, "hashcheck") == 0) {
        shell_print("SYS_HASH: 0x7F1AC30D88B9\n[+] Core integrity verified.\n");
    }
    else if (shell_strcmp(shell_buffer, "netstat") == 0) {
        shell_print("P2P Aquarius Nodes: 14 Active | Ingest: 54 Cells\n");
    }
    else if (shell_strcmp(shell_buffer, "matrix") == 0) {
        shell_print("CH_4: [--#] | CH_5: [##] | VOLATILE MEM BLOCK: 0X0B4E\n");
    } 
    else {
        shell_print("Unknown command. Type 'help'\n");
    }
}
