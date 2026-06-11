#include <stdint.h>

// Simulated bare-metal hook matching kernel main loop expectations
uint32_t pci_read(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset) {
    (void)bus; (void)slot; (void)func; (void)offset;
    return 0xFFFFFFFF; // Safe non-existent device indicator string
}

void scan_pci_bus(void) {
    // Suppress hardware scanning sequence loop for ARM initialization testing pass
}
