#include <stdint.h>
#include <stddef.h>

#define VGA_ADDRESS 0xB8000
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

// Standard VGA Color Attribute flags
#define COLOR_WHITE_ON_BLACK 0x07

static uint16_t* const vga_buffer = (uint16_t*)VGA_ADDRESS;
static int cursor_x = 0;
static int cursor_y = 0;

// Completely wipes the physical text buffer grid screen clean
void vga_clear_screen(void) {
    for (int y = 0; y < VGA_HEIGHT; y++) {
        for (int x = 0; x < VGA_WIDTH; x++) {
            const int index = y * VGA_WIDTH + x;
            vga_buffer[index] = (COLOR_WHITE_ON_BLACK << 8) | ' ';
        }
    }
    cursor_x = 0;
    cursor_y = 0;
}

// Low-level video driver to output a single character text block
void vga_putc(char c) {
    // Handle standard newlines cleanly
    if (c == '\n') {
        cursor_x = 0;
        cursor_y++;
    } else {
        const int index = cursor_y * VGA_WIDTH + cursor_x;
        vga_buffer[index] = (COLOR_WHITE_ON_BLACK << 8) | c;
        cursor_x++;
    }

    // Wrap text lines automatically if hitting the right monitor bounds
    if (cursor_x >= VGA_WIDTH) {
        cursor_x = 0;
        cursor_y++;
    }

    // Scroll screen safely if writing passes the bottom margin bounds
    if (cursor_y >= VGA_HEIGHT) {
        for (int i = 0; i < (VGA_HEIGHT - 1) * VGA_WIDTH; i++) {
            vga_buffer[i] = vga_buffer[i + VGA_WIDTH];
        }
        for (int i = (VGA_HEIGHT - 1) * VGA_WIDTH; i < VGA_HEIGHT * VGA_WIDTH; i++) {
            vga_buffer[i] = (COLOR_WHITE_ON_BLACK << 8) | ' ';
        }
        cursor_y = VGA_HEIGHT - 1;
    }
}
