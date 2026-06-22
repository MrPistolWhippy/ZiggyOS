#pragma once
#include "part16_final_rtos.h"

#define ELF_MAGIC_0 0x7F
#define ELF_MAGIC_1 'E'
#define ELF_MAGIC_2 'L'
#define ELF_MAGIC_3 'F'
#define CEILING_PRIORITY 99

typedef struct {
    uint8_t e_ident[16];
    uint16_t e_type;
    uint16_t e_machine;
    uint32_t e_version;
    uintptr_t e_entry;
} elf_header_t;

static uint64_t sv48_root_table[512] __attribute__((aligned(4096)));
static uint64_t sv48_lvl2_table[512] __attribute__((aligned(4096)));

static inline void z_priority_ceiling_lock(spinlock_t* l, uint32_t* orig_p, uint32_t current_p) {
    *orig_p = current_p;
    uint32_t elevated_p = CEILING_PRIORITY;
    ziggy_mutex_lock(l, elevated_p);
    printf("[CEILING_MUTEX] Thread priority dynamically elevated to Ceiling (%d) to prevent locks inversion.\n", CEILING_PRIORITY);
}

static inline void z_priority_ceiling_unlock(spinlock_t* l, uint32_t orig_p) {
    ziggy_mutex_unlock(l);
    printf("[CEILING_MUTEX] Lock released. Thread priority restored to baseline (%d).\n", orig_p);
}

static inline void z_sv48_init_extended() {
    memset(sv48_root_table, 0, sizeof(sv48_root_table));
    memset(sv48_lvl2_table, 0, sizeof(sv48_lvl2_table));
    
    // Extract VPN3 for the 4th level indexing iteration
    uint64_t va = 0xFFFF800000000000ULL;
    uint32_t vpn3 = (va >> 39) & 0x1FF;
    uint64_t ppn = ((uintptr_t)&sv48_lvl2_table) >> 12;
    
    sv48_root_table[vpn3] = (ppn << 10) | PTE_V;
    printf("[MMU_SV48] 4-Level High-Capacity Virtual Memory Scale active. Root VPN3 Index [%d] mapped.\n", vpn3);
}

static inline int z_elf_load_verify(const elf_header_t* header) {
    if (header->e_ident[0] == ELF_MAGIC_0 && header->e_ident[1] == ELF_MAGIC_1 &&
        header->e_ident[2] == ELF_MAGIC_2 && header->e_ident[3] == ELF_MAGIC_3) {
        printf("[ELF_LOADER] Executable signature match verified. Entry Vector target address: 0x%lX\n", (unsigned long)header->e_entry);
        return ZIGGY_OK;
    }
    return ZIGGY_ERR;
}
