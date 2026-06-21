#include <stdint.h>
#include <stddef.h>

#define BUFFER_SIZE 256

extern void vga_putc(char c);

static char shell_buffer[BUFFER_SIZE];
static int buffer_index = 0;

void shell_print(const char* str) {
    for (size_t i = 0; str[i] != '\0'; i++) {
        vga_putc(str[i]);
    }
}

void shell_clear_buffer(void) {
    for (int i = 0; i < BUFFER_SIZE; i++) {
        shell_buffer[i] = '\0';
    }
    buffer_index = 0;
}

int shell_strcmp(const char* s1, const char* s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}

void shell_execute_command(void) {
    shell_print("\n");
    if (buffer_index == 0) {
        shell_print("ZiggyOS> ");
        return;
    }
    
    if (shell_strcmp(shell_buffer, "help") == 0) {
        shell_print("Commands: help, clear, status, info\n");
    } else if (shell_strcmp(shell_buffer, "clear") == 0) {
        extern void vga_clear_screen(void);
        vga_clear_screen();
    } else if (shell_strcmp(shell_buffer, "status") == 0) {
        shell_print("Kernel: Sandbox Stable | Multitasking: Enabled\n");
    } else if (shell_strcmp(shell_buffer, "info") == 0) {
        shell_print("OS Architecture: x86 freestanding i386\n");
        shell_print("Kernel Core Base: 0x100000 | Video Memory: 0xB8000\n");
    } else {
        shell_print("Unknown Command. Type 'help' for instructions.\n");
    }
    
    shell_clear_buffer();
    shell_print("ZiggyOS> ");
}

void shell_input_char(char c) {
    if (c == '\n') {
        shell_execute_command();
        return;
    }
    if (c == '\b') {
        if (buffer_index > 0) {
            buffer_index--;
            shell_buffer[buffer_index] = '\0';
            vga_putc(c);
        }
        return;
    }
    if (buffer_index < BUFFER_SIZE - 1) {
        shell_buffer[buffer_index] = c;
        buffer_index++;
        vga_putc(c);
    }
}
