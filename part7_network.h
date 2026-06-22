#pragma once
#include "part6_shell.h"

#define NET_BUFFER_SIZE 256
#define NET_SECRET_XOR 0x3F

typedef struct {
    uint16_t src_port;
    uint16_t dest_port;
    uint8_t payload[NET_BUFFER_SIZE];
    uint32_t payload_len;
} socket_frame_t;

void z_net_transmit_secure(socket_frame_t* frame, uint8_t* wire_out) {
    // Encapsulate packet header layers
    wire_out[0] = (frame->src_port >> 8) & 0xFF;
    wire_out[1] = frame->src_port & 0xFF;
    wire_out[2] = (frame->dest_port >> 8) & 0xFF;
    wire_out[3] = frame->dest_port & 0xFF;
    
    // Encrypt transport data block inline
    for (uint32_t i = 0; i < frame->payload_len; i++) {
        wire_out[4 + i] = frame->payload[i] ^ NET_SECRET_XOR;
    }
}
