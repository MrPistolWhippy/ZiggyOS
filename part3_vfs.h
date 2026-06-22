#pragma once
#include "part2.h"

#define VFS_MAX_LOG 1024
#define VFS_CIPHER_KEY 0xA5

typedef struct {
    char encrypted_buffer[VFS_MAX_LOG];
    uint32_t write_ptr;
    uint32_t mode_permissions;
} vnode_t;

static vnode_t kernel_log_vnode;

void z_vfs_init() {
    memset(&kernel_log_vnode, 0, sizeof(vnode_t));
    kernel_log_vnode.mode_permissions = 0600; // Owner read/write isolated
}

int z_vfs_log_write(const char* plain_text) {
    uint32_t len = strlen(plain_text);
    if (kernel_log_vnode.write_ptr + len >= VFS_MAX_LOG) return ZIGGY_ERR;
    
    for (uint32_t i = 0; i < len; i++) {
        // VENCL Stream Layer Encryption Primitive
        kernel_log_vnode.encrypted_buffer[kernel_log_vnode.write_ptr] = plain_text[i] ^ VFS_CIPHER_KEY;
        kernel_log_vnode.write_ptr++;
    }
    return ZIGGY_OK;
}

void z_vfs_dump_decrypted() {
    printf("[VFS] Decrypting Secure Virtual Vnode Storage Stream (VENCDL):\n  -> ");
    for (uint32_t i = 0; i < kernel_log_vnode.write_ptr; i++) {
        char decrypted_char = kernel_log_vnode.encrypted_buffer[i] ^ VFS_CIPHER_KEY;
        putchar(decrypted_char);
    }
    printf("\n");
}
