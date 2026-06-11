#include <stdint.h>

// Direct pointer to the x86 VGA text memory buffer
static uint16_t* const vga_buffer = (uint16_t*)0xB8000;
static int vga_column = 0;
static int vga_row = 0;

void print(const char* str) {
    for (int i = 0; str[i] != '\0'; i++) {
        char c = str[i];
        
        // Handle basic newlines
        if (c == '\n') {
            vga_column = 0;
            vga_row++;
            continue;
        }
        
        // Calculate buffer array index (80 columns x 25 rows grid layout)
        int index = (vga_row * 80) + vga_column;
        
        // Write the character with a standard light-grey text color attribute (0x07)
        vga_buffer[index] = (uint16_t)c | (0x07 << 8);
        
        vga_column++;
        if (vga_column >= 80) {
            vga_column = 0;
            vga_row++;
        }
    }
}
