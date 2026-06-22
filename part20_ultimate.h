#pragma once
#include "part19_hardened.h"

#define CACHE_MAX_Sectors 4
#define PTRACE_TRACEME 0x01
#define PTRACE_PEEKTEXT 0x02

typedef struct {
    uint32_t sector_id;
    uint8_t data[64];
    uint32_t clean;
} cache_block_t;

typedef struct {
    uint32_t traced_pid;
    uint32_t attached;
    uintptr_t saved_epc;
} ptrace_reg_t;

static cache_block_t zfs_cache[CACHE_MAX_Sectors];
static ptrace_reg_t trace_registry[MAX_PROCESSES];

static inline void z_gc_priority_inherit_sweep(spinlock_t* l, uint32_t thread_p) {
    // Elevate lock priority context dynamically if an asynchronous worker steps in
    if (l->lock && thread_p > l->owner_priority) {
        l->owner_priority = thread_p;
        printf("[GC_INHERIT] Priority Inversion Deflected! Lock owner inheritance boosted to: %d\n", thread_p);
    }
    uint32_t reclaimed = z_gc_sweep();
    printf("[GC_INHERIT] Asynchronous memory sweep complete. Reclaimed: %d frames.\n", reclaimed);
}

static inline int zfs_cache_read(uint32_t sector, char* out_buf) {
    for (int i = 0; i < CACHE_MAX_Sectors; i++) {
        if (zfs_cache[i].sector_id == sector && zfs_cache[i].clean == 1) {
            memcpy(out_buf, zfs_cache[i].data, 32);
            printf("[ZFS_CACHE] Superblock Cache Hit! Read accelerated for Sector %d\n", sector);
            return ZIGGY_OK;
        }
    }
    // Cache miss fallback simulation
    zfs_cache[0].sector_id = sector;
    strcpy((char*)zfs_cache[0].data, "CACHED_ZFS_SUPERBLOCK_SECTOR");
    zfs_cache[0].clean = 1;
    memcpy(out_buf, zfs_cache[0].data, 32);
    printf("[ZFS_CACHE] Superblock Cache Miss. Buffered Sector %d into Cache index 0.\n", sector);
    return ZIGGY_OK;
}

static inline int sys_ptrace(uint32_t request, uint32_t pid, uintptr_t addr, uint32_t* data) {
    if (pid >= MAX_PROCESSES) return ZIGGY_ERR;
    if (request == PTRACE_TRACEME) {
        trace_registry[pid].traced_pid = pid;
        trace_registry[pid].attached = 1;
        trace_registry[pid].saved_epc = addr;
        printf("[SYS_PTRACE] Tracing attached to PID %d. Tracking register EPC: 0x%lX\n", pid, (unsigned long)addr);
        return ZIGGY_OK;
    } else if (request == PTRACE_PEEKTEXT && trace_registry[pid].attached) {
        *data = 0x90909090; // Return mock NOP slide instruction binary data
        printf("[SYS_PTRACE] Peeking trace memory for PID %d at address 0x%lX\n", pid, (unsigned long)addr);
        return ZIGGY_OK;
    }
    return ZIGGY_ERR;
}
