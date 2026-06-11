#include "io.h"

int keyboard_has_key() {
    // Read the PS/2 Status Register Port 0x64 and check bit 0 (Output Buffer Full)
    return (inb(0x64) & 0x01);
}

unsigned char keyboard_read() {
    // Read the PS/2 Data Port 0x60 to pull the hardware keyboard scancode byte
    return inb(0x60);
}
