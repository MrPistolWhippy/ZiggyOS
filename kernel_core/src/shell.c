#include "io.h"

unsigned int cursor_pos = 0;
volatile unsigned short* video_memory = (volatile unsigned short*)0xB8000;
char current_color = 0x0F;

void print_char(char c) {
    if (c == '\n') {
        cursor_pos = ((cursor_pos / 80) + 1) * 80;
    } else {
        video_memory[cursor_pos] = (current_color << 8) | c;
        cursor_pos++;
    }
}

void print(const char* str) {
    for (int i = 0; str[i] != '\0'; i++) print_char(str[i]);
}

void shell_clear() {
    for (int i = 0; i < 2000; i++) video_memory[i] = (current_color << 8) | ' ';
    cursor_pos = 0;
}

void shell_backspace() {
    if (cursor_pos > 0) { cursor_pos--; video_memory[cursor_pos] = (current_color << 8) | ' '; }
}
