#pragma once
#include "part15_pi_surpass.h"

#define JRNL_MAX_RECORDS 2
#define ENC_MBOX_KEY 0x7A

typedef struct {
    uintptr_t faulting_epc;
    uintptr_t faulting_badvaddr;
    uint32_t cause_code;
} trap_frame_t;

typedef struct {
    uint32_t transaction_id;
    uint32_t encrypted_hash;
    uint8_t payload[16];
} enc_mailbox_t;

typedef struct {
    uint32_t sector_id;
    char operation_type[8];
    uint32_t committed;
} z_journal_t;

static z_journal_t disk_journal[JRNL_MAX_RECORDS];
static uint32_t journal_ptr = 0;

static inline void z_trap_vector(trap_frame_t* tf) {
    printf("[TRAP] Hardware Exception Caught! Cause Code: 0x%02X | Faulting EPC: 0x%lX\n", 
           tf->cause_code, (unsigned long)tf->faulting_epc);
}

static inline void z_mailbox_send_secure(uint32_t tx_id, const uint8_t* data, enc_mailbox_t* out_box) {
    out_box->transaction_id = tx_id;
    uint32_t hash = 2166136261U;
    for (int i = 0; i < 16; i++) {
        out_box->payload[i] = data[i] ^ ENC_MBOX_KEY;
        hash ^= out_box->payload[i];
        hash *= 16777619;
    }
    out_box->encrypted_hash = hash;
    printf("[ENC_MBOX] Inter-processor message encrypted. Verification Hash: 0x%08X\n", hash);
}

static inline void z_journal_log(uint32_t sector, const char* op) {
    uint32_t idx = journal_ptr % JRNL_MAX_RECORDS;
    disk_journal[idx].sector_id = sector;
    strncpy(disk_journal[idx].operation_type, op, 7);
    disk_journal[idx].committed = 1;
    journal_ptr++;
    printf("[JOURNAL] Meta-transaction logged. Target Sector: %d [COMMITTED]\n", sector);
}
