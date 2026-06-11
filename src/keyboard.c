#include "io.h"

unsigned char keyboard_read() {
    // Wait for the keyboard controller buffer to fill up (Status Port 0x64, Bit 0)
    while ((inb(0x64) & 1) == 0);
    // Read the scan code from the Data Port (0x60)
    return inb(0x60);
}

int keyboard_has_key() {
	return (inb(0x64) \& 1);
}
