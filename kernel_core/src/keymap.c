#include "io.h"

char scancode_to_ascii(unsigned char scancode) {
    // Basic translation array mapper logic for alpha keys
    if (scancode == 0x1E) return 'a';
    if (scancode == 0x30) return 'b';
    if (scancode == 0x2E) return 'c';
    if (scancode == 0x20) return 'd';
    if (scancode == 0x12) return 'e';
    if (scancode == 0x21) return 'f';
    if (scancode == 0x22) return 'g';
    if (scancode == 0x23) return 'h';
    if (scancode == 0x17) return 'i';
    if (scancode == 0x24) return 'j';
    if (scancode == 0x25) return 'k';
    if (scancode == 0x26) return 'l';
    if (scancode == 0x32) return 'm';
    if (scancode == 0x31) return 'n';
    if (scancode == 0x18) return 'o';
    if (scancode == 0x19) return 'p';
    if (scancode == 0x10) return 'q';
    if (scancode == 0x13) return 'r';
    if (scancode == 0x1F) return 's';
    if (scancode == 0x14) return 't';
    if (scancode == 0x16) return 'u';
    if (scancode == 0x2F) return 'v';
    if (scancode == 0x11) return 'w';
    if (scancode == 0x2D) return 'x';
    if (scancode == 0x15) return 'y';
    if (scancode == 0x2C) return 'z';
    if (scancode == 0x39) return ' '; // Space bar scancode
    if (scancode == 0x1C) return '\n'; // Enter key scancode
    if (scancode == 0x0E) return '\b'; // Backspace scancode
    return 0;
}
