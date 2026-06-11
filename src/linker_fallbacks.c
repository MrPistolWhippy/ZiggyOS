#include <stdint.h>

// Fallback direct printer implementation
void print(const char* str) { (void)str; }

// Missing storage driver implementation fallback
int flash_read_sector(uint32_t sector, uint8_t* buffer) {
    (void)sector;
    (void)buffer;
    return 0; // Return success to allow filesystem init simulation
}
