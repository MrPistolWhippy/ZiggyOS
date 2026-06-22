#pragma once
#include "part11_internet.h"
#define MAX_PAGES 16
#define PAGE_REF_BIT (1U << 0)
typedef struct { uintptr_t phys_addr; uint32_t flags; uint32_t ref_count; } page_frame_t;
static page_frame_t frame_table[MAX_PAGES];
static uint32_t gc_sweep_count = 0;
static void z_gc_init() {
    memset(frame_table, 0, sizeof(frame_table));
    for (int i = 0; i < MAX_PAGES; i++) {
        frame_table[i].phys_addr = (uintptr_t)&kmem_pool[i * 16];
        frame_table[i].flags = PAGE_REF_BIT;
        frame_table[i].ref_count = 1;
    }
}
static uint32_t z_gc_sweep() {
    uint32_t pages_reclaimed = 0; gc_sweep_count++;
    for (int i = 0; i < MAX_PAGES; i++) {
        if (frame_table[i].ref_count > 0) {
            frame_table[i].ref_count--;
            frame_table[i].flags &= ~PAGE_REF_BIT;
        } else if (!(frame_table[i].flags & PAGE_REF_BIT)) {
            pages_reclaimed++;
            frame_table[i].flags = 0;
            if(allocator.free_buckets[4] < 32) allocator.free_buckets[4]++;
        }
    }
    return pages_reclaimed;
}
