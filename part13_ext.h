#pragma once
#include "part12_gc.h"

#define MAX_DIR_ENTRIES 4
#define ENTROPY_POOL_SZ 16

typedef struct {
    char name[16];
    uint32_t start_block;
    uint32_t size_bytes;
} zfs_dir_entry_t;

static zfs_dir_entry_t current_dir[MAX_DIR_ENTRIES];
static uint32_t entropy_pool[ENTROPY_POOL_SZ];
static uint32_t entropy_ptr = 0;

static inline void z_fs_init() {
    strcpy(current_dir[0].name, "."); current_dir[0].start_block = 1;
    strcpy(current_dir[1].name, ".."); current_dir[1].start_block = 1;
    strcpy(current_dir[2].name, "kernel.sys"); current_dir[2].start_block = 2; current_dir[2].size_bytes = 16384;
    strcpy(current_dir[3].name, "network.cfg"); current_dir[3].start_block = 3; current_dir[3].size_bytes = 512;
}

static inline void zfs_ls() {
    printf("[ZFS_WALKER] Indexing directory layout tree node map:\n");
    for (int i = 0; i < MAX_DIR_ENTRIES; i++) {
        printf("  -> Entry: %-12s | Block Start: %-3d | Size: %d Bytes\n", 
               current_dir[i].name, current_dir[i].start_block, current_dir[i].size_bytes);
    }
}

static inline int zfs_cd(const char* target_dir) {
    for (int i = 0; i < MAX_DIR_ENTRIES; i++) {
        if (strcmp(current_dir[i].name, target_dir) == 0) {
            printf("[ZFS_WALKER] Verification complete. Target cluster locked: %s\n", target_dir);
            return ZIGGY_OK;
        }
    }
    return ZIGGY_ERR;
}

static inline void z_entropy_harvest() {
    uint32_t seed = 0xACE1U;
    for (int i = 0; i < ENTROPY_POOL_SZ; i++) {
        // Linear feedback register hardware entropy extraction approximation
        seed = (seed >> 1) ^ (-(seed & 1u) & 0xB400u);
        entropy_pool[i] = seed ^ (uint32_t)usleep(1);
    }
}

static inline uint32_t z_get_random() {
    entropy_ptr = (entropy_ptr + 1) % ENTROPY_POOL_SZ;
    return entropy_pool[entropy_ptr];
}

static inline void z_oaep_pad(const uint8_t* msg, uint32_t m_len, uint8_t* out_padded) {
    uint32_t mask = z_get_random();
    memset(out_padded, 0, 32);
    memcpy(out_padded, msg, m_len > 16 ? 16 : m_len);
    out_padded[16] = 0x01; // Append explicit boundary flag delimiter
    
    for (int i = 0; i < 32; i++) {
        out_padded[i] ^= ((mask >> ((i % 4) * 8)) & 0xFF);
    }
}
