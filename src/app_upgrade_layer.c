#include <stdint.h>
#include <stddef.h>

extern void uart_putc(char c);
extern volatile uint64_t system_jiffies_ticks;

// --- Subsystem 1: Freestanding 1KB Kernel Bump Allocator (kmalloc) ---
#define HEAP_SIZE 1024
static uint8_t kernel_heap[HEAP_SIZE];
static size_t heap_offset = 0;


// --- Subsystem 2: High-Efficiency Telemetry Bitmask Compressor ---
void app_compress_and_broadcast_telemetry(void) {
    // Dynamic allocation of a scratchpad packet buffer using our new kmalloc
    uint8_t *packet = (uint8_t*)kmalloc(4);
    if (!packet) return;

    uint8_t lattice_token = 43; 
    uint32_t ticks = (uint32_t)(system_jiffies_ticks & 0xFFFFF); // Clamp ticks to 20 bits

    // Bitmask Compression Layout (Total: 32 bits / 4 bytes):
    // [ Bits 0-7: Lattice Token (8 bits) ] [ Bits 8-27: System Clock Ticks (20 bits) ] [ Bits 28-31: Padding/Flags (4 bits) ]
    packet[0] = lattice_token;
    packet[1] = (uint8_t)(ticks & 0xFF);
    packet[2] = (uint8_t)((ticks >> 8) & 0xFF);
    packet[3] = (uint8_t)(((ticks >> 16) & 0x0F) | 0xA0); // Lower 4 bits of tick remainder + 0xA marker flag

    // Broadcast the compressed bitstream byte-by-byte natively over the MMIO serial line
    uart_putc('[');
    for (int i = 0; i < 4; i++) {
        // Output as raw ASCII-shifted visual hex tokens for the current terminal view
        char hex_high = ((packet[i] >> 4) & 0x0F) + 'A';
        char hex_low = (packet[i] & 0x0F) + 'A';
        uart_putc(hex_high);
        uart_putc(hex_low);
    }
    uart_putc(']');
}
