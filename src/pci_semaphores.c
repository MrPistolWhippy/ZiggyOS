#include <stdint.h>

extern void print(const char* str);
extern void print_hex_byte(uint8_t b);

// Direct assembly port out/in wrappers
static inline void outl(uint16_t port, uint32_t val) {
    asm volatile("outl %0, %1" : : "a"(val), "Nd"(port));
}

static inline uint32_t inl(uint16_t port) {
    uint32_t ret;
    asm volatile("inl %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

// Read raw configuration attributes from target hardware bus vectors
uint32_t pci_config_read(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset) {
    uint32_t address;
    uint32_t lbus  = (uint32_t)bus;
    uint32_t lslot = (uint32_t)slot;
    uint32_t lfunc = (uint32_t)func;
    
    // Construct target configurations register layout address token bitfield
    address = (uint32_t)((lbus << 16) | (lslot << 11) | (lfunc << 8) | (offset & 0xFC) | ((uint32_t)0x80000000));
    
    outl(0xCF8, address);
    return inl(0xCFC);
}

void probe_pci_bus_sys(void) {
    print("PCI: Initiating peripheral configuration bus scan...\n");
    
    // Scan common low-level device slot arrays
    for (uint8_t bus = 0; bus < 8; bus++) {
        for (uint8_t slot = 0; slot < 32; slot++) {
            uint32_t reg0 = pci_config_read(bus, slot, 0, 0);
            uint16_t vendor_id = reg0 & 0xFFFF;
            
            // If Vendor ID is not 0xFFFF, a valid peripheral device is present!
            if (vendor_id != 0xFFFF) {
                uint16_t device_id = (reg0 >> 16) & 0xFFFF;
                print("  - Found Device! Bus: ");
                print_hex_byte(bus);
                print(" Slot: ");
                print_hex_byte(slot);
                print(" | Vendor: [0x");
                print_hex_byte((vendor_id >> 8) & 0xFF);
                print_hex_byte(vendor_id & 0xFF);
                print("]\n");
            }
        }
    }
}
