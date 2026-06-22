#pragma once
#include "part11_internet.h"

#define MAX_PAGES 16
#define PAGE_REF_BIT (1U << 0)
#define PAGE_DIRTY_BIT (1U << 1)

typedef struct {
    uintptr_t phys_addr;
    uint32_t flags;
    uint32_t ref_count;
} page_frame_t;

static page_frame_t frame_table[MAX_PAGES];
static uint32_t gc_sweep_count = 0;

static inline void z_gc_init() {
    memset(frame_table, 0, sizeof(frame_table));
    for (int i = 0; i < MAX_PAGES; i++) {
        frame_table[i].phys_addr = (uintptr_t)&kmem_pool[i * 32];
        frame_table[i].flags = PAGE_REF_BIT; // Initialize as recently referenced
        frame_table[i].ref_count = 1;
    }
}

static inline uint32_t z_gc_sweep() {
    uint32_t pages_reclaimed = 0;
    gc_sweep_count++;
    
    for (int i = 0; i < MAX_PAGES; i++) {
        // Clock-sweep aging architecture approximation
        if (frame_table[i].ref_count > 0) {
            frame_table[i].ref_count--;
            frame_table[i].flags &= ~PAGE_REF_BIT; // Clear reference bit flag
        } else if (!(frame_table[i].flags & PAGE_REF_BIT)) {
            // Reclaim frame if reference score hits zero bounds
            pages_reclaimed++;
            frame_table[i].flags = 0;
            // Return tracking slice index block to allocator buckets dynamically
            allocator.free_buckets[4]++;
        }
    }
    return pages_reclaimed;
}
