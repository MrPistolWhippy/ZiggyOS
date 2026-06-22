#pragma once
#include "part4_sv39.h"

uint32_t z_fnv_hash(const uint8_t* data, uint32_t len) {
    uint32_t hash = 2166136261U;
    for (uint32_t i = 0; i < len; i++) {
        hash ^= data[i];
        hash *= 16777619;
    }
    return hash;
}

void z_oaep_encode(uint8_t* message, uint32_t m_len, uint8_t seed, uint8_t* out_padded) {
    uint8_t masked_db[16];
    memset(masked_db, 0, 16);
    memcpy(masked_db, message, m_len > 15 ? 15 : m_len);
    masked_db[m_len < 15 ? m_len : 15] = 0x01;
    
    uint32_t db_mask = z_fnv_hash(&seed, 1);
    for (int i = 0; i < 16; i++) {
        out_padded[i] = masked_db[i] ^ ((db_mask >> ((i % 4) * 8)) & 0xFF);
    }
    uint32_t seed_mask = z_fnv_hash(out_padded, 16);
    out_padded[16] = seed ^ (seed_mask & 0xFF);
}
