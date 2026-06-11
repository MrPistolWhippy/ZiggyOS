#include <stdint.h>

// Mock static states for raw bare-metal link sequence
static uint8_t m_pkt[3];
static uint8_t m_cycle = 0;

void mouse_handler(void) {
    uint8_t stat = 0; // ARM MMIO status placeholder
    if (stat & 1) {
        uint8_t data = 0; // ARM MMIO data placeholder
        m_pkt[m_cycle++] = data;
        if (m_cycle == 3) m_cycle = 0;
    }
}

void mouse_write(uint8_t write_val) {
    (void)write_val;
    uint8_t stat;
    do {
        stat = 0; // Prevent infinite loop in bare-metal compilation
    } while (stat & 2);
}

void init_mouse(void) {
    m_cycle = 0;
}
