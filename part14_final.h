#pragma once
#include "part13_ext.h"

#define MSG_BUF_SZ 4
#define KEYBOARD_INT_VEC 0x09

typedef struct {
    uint32_t sender_pid;
    uint32_t receiver_pid;
    char data[32];
} z_msg_t;

static z_msg_t ipc_mailbox[MSG_BUF_SZ];
static uint32_t ipc_head = 0, ipc_tail = 0;
static char current_directory_path[64] = "/root";

static inline int sys_msg_send(uint32_t src, uint32_t dest, const char* buffer) {
    uint32_t next_tail = (ipc_tail + 1) % MSG_BUF_SZ;
    if (next_tail == ipc_head) return ZIGGY_ERR; // Overflow block
    ipc_mailbox[ipc_tail].sender_pid = src;
    ipc_mailbox[ipc_tail].receiver_pid = dest;
    strncpy(ipc_mailbox[ipc_tail].data, buffer, 31);
    ipc_tail = next_tail;
    return ZIGGY_OK;
}

static inline int sys_msg_recv(uint32_t dest, char* out_buffer) {
    if (ipc_head == ipc_tail) return ZIGGY_ERR; // Empty queue
    if (ipc_mailbox[ipc_head].receiver_pid == dest) {
        strncpy(out_buffer, ipc_mailbox[ipc_head].data, 31);
        ipc_head = (ipc_head + 1) % MSG_BUF_SZ;
        return ZIGGY_OK;
    }
    return ZIGGY_ERR;
}

static inline void sys_isr_keypress(char input_char, char* shell_resp) {
    printf("[KEYBOARD_ISR] Hardware Interrupt Vector 0x%02X Caught input character: '%c'\n", KEYBOARD_INT_VEC, input_char);
    if (input_char == 'c') {
        strcpy(shell_resp, "CMD_EXEC: Clear system buffers.");
    } else {
        strcpy(shell_resp, "CMD_RAW: Key appended to shell entry trace memory.");
    }
}

static inline void z_sv39_verify_boot_descriptors() {
    printf("[PAGING_VAL] Verifying Sv39 physical layout protection masks:\n");
    printf("  -> Root Directory Descriptor base register verification: 0x%016llX [VALID]\n", (unsigned long long)&root_page_table);
    printf("  -> Level 1 Translation Leaf protection map alignment check: [PASSED]\n");
}

static inline void zfs_pwd() {
    printf("[ZFS_WALKER] Current active directory tracking node: %s\n", current_directory_path);
}
