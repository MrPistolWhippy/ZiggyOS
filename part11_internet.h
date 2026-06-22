#include "part10_decay.h"

#define SLIP_END     0xC0
#define SLIP_ESC     0xDB
#define SLIP_ESC_END 0xDC
#define SLIP_ESC_ESC 0xDD

uint32_t z_slip_encapsulate(const uint8_t* raw_ip_pkt, uint32_t pkt_len, uint8_t* out_wire) {
    uint32_t ptr = 0;
    out_wire[ptr++] = SLIP_END;
    
    for (uint32_t i = 0; i < pkt_len; i++) {
        if (raw_ip_pkt[i] == SLIP_END) {
            out_wire[ptr++] = SLIP_ESC;
            out_wire[ptr++] = SLIP_ESC_END;
        } else if (raw_ip_pkt[i] == SLIP_ESC) {
            out_wire[ptr++] = SLIP_ESC;
            out_wire[ptr++] = SLIP_ESC_ESC;
        } else {
            out_wire[ptr++] = raw_ip_pkt[i];
        }
    }
    out_wire[ptr++] = SLIP_END;
    return ptr; // Returns length serialized to serial line interface
}
