#pragma once
#include "part3_vfs.h"

#define PTE_V (1ULL << 0) // Valid Descriptor Bit
#define PTE_R (1ULL << 1) // Read Permission Bit
#define PTE_W (1ULL << 2) // Write Permission Bit
#define PTE_X (1ULL << 3) // Execute Permission Bit

typedef uint64_t pte_t;

typedef struct {
    pte_t entries[512]; 
} page_table_t;

// Static allocation within the sandbox kernel architecture heap
static page_table_t root_page_table __attribute__((aligned(4096)));
static page_table_t level1_page_table __attribute__((aligned(4096)));

void z_sv39_init() {
    memset(&root_page_table, 0, sizeof(page_table_t));
    memset(&level1_page_table, 0, sizeof(page_table_t));
}

int z_sv39_map_page(uint64_t va, uint64_t pa, uint64_t flags) {
    // Extract VPN indexes for level 2 and level 1 descriptors
    uint32_t vpn2 = (va >> 30) & 0x1FF;
    uint32_t vpn1 = (va >> 21) & 0x1FF;

    // Link level 2 descriptor to lower page table base pointer
    uint64_t ppn = ((uintptr_t)&level1_page_table) >> 12;
    root_page_table.entries[vpn2] = (ppn << 10) | PTE_V;

    // Conclude step inside level 1 directory mapped directly to physical address
    uint64_t target_ppn = pa >> 12;
    level1_page_table.entries[vpn1] = (target_ppn << 10) | flags | PTE_V;

    return ZIGGY_OK;
}

uint64_t z_sv39_translate(uint64_t va) {
    uint32_t vpn2 = (va >> 30) & 0x1FF;
    uint32_t vpn1 = (va >> 21) & 0x1FF;
    uint64_t offset = va & 0x1FFFFF; // 2MB Huge Page Boundary match

    if (!(root_page_table.entries[vpn2] & PTE_V)) return 0;
    pte_t pte1 = level1_page_table.entries[vpn1];
    if (!(pte1 & PTE_V)) return 0;

    uint64_t ppn = pte1 >> 10;
    return (ppn << 12) + offset;
}
