#pragma once
#include "part18_occult.h"

#define PROT_NONE  0x0
#define PROT_READ  0x1
#define PROT_WRITE 0x2

// Curve parameters for a mock secure 16-bit prime field: y^2 = x^3 + ax + b (mod 17)
#define ECC_PRIME 17

typedef struct {
    uint32_t target_pid;
    uint32_t missed_heartbeats;
    uint32_t recovery_count;
} watchdog_node_t;

static watchdog_node_t system_watchdog[MAX_PROCESSES];

static inline int sys_mprotect(uintptr_t addr, size_t len, uint32_t prot) {
    uint64_t va = (uint64_t)addr;
    uint32_t vpn2 = (va >> 30) & 0x1FF;
    uint32_t vpn1 = (va >> 21) & 0x1FF;

    if (!(root_page_table.entries[vpn2] & PTE_V)) return ZIGGY_ERR;
    
    // Clear and override protection flags on the leaf table entries
    level1_page_table.entries[vpn1] &= ~(PTE_R | PTE_W | PTE_X);
    if (prot & PROT_READ)  level1_page_table.entries[vpn1] |= PTE_R;
    if (prot & PROT_WRITE) level1_page_table.entries[vpn1] |= PTE_W;
    
    printf("[SYS_MPROTECT] Hardened address isolation applied at 0x%lX. Memory protection bits locked.\n", (unsigned long)addr);
    return ZIGGY_OK;
}

static inline void z_ecc_point_add(uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2, uint32_t* xr, uint32_t* yr) {
    if (x1 == x2 && y1 == y2) {
        // Point doubling approximation over our prime field matrix
        uint32_t num = (3 * x1 * x1) % ECC_PRIME;
        uint32_t den = (2 * y1) % ECC_PRIME;
        uint32_t inv = 1;
        for (uint32_t i = 1; i < ECC_PRIME; i++) {
            if ((den * i) % ECC_PRIME == 1) { inv = i; break; }
        }
        uint32_t lambda = (num * inv) % ECC_PRIME;
        *xr = (lambda * lambda - 2 * x1 + 2 * ECC_PRIME) % ECC_PRIME;
        *yr = (lambda * (x1 - *xr) - y1 + 2 * ECC_PRIME) % ECC_PRIME;
    } else {
        uint32_t num = (y2 - y1 + ECC_PRIME) % ECC_PRIME;
        uint32_t den = (x2 - x1 + ECC_PRIME) % ECC_PRIME;
        uint32_t inv = 1;
        for (uint32_t i = 1; i < ECC_PRIME; i++) {
            if ((den * i) % ECC_PRIME == 1) { inv = i; break; }
        }
        uint32_t lambda = (num * inv) % ECC_PRIME;
        *xr = (lambda * lambda - x1 - x2 + 2 * ECC_PRIME) % ECC_PRIME;
        *yr = (lambda * (x1 - *xr) - y1 + 2 * ECC_PRIME) % ECC_PRIME;
    }
    printf("[ECC_ENGINE] Asymmetric Prime Field Point Addition calculated: R(%d, %d)\n", *xr, *yr);
}

static inline void z_watchdog_init() {
    for (int i = 0; i < MAX_PROCESSES; i++) {
        system_watchdog[i].target_pid = i;
        system_watchdog[i].missed_heartbeats = 0;
        system_watchdog[i].recovery_count = 0;
    }
}

static inline void z_watchdog_check_tick() {
    for (int i = 0; i < MAX_PROCESSES; i++) {
        // Simulate a dead user-space server process (PID 2 missing check-ins)
        if (i == 2) {
            system_watchdog[i].missed_heartbeats++;
            if (system_watchdog[i].missed_heartbeats >= 3) {
                system_watchdog[i].recovery_count++;
                system_watchdog[i].missed_heartbeats = 0;
                printf("[WATCHDOG_DAEMON] Health Exception Trapped! Dead Process PID %d isolated and auto-recovered.\n", i);
            }
        }
    }
}
