#pragma once
#include "part8_uart.h"

#define BLK_SIZE 512
#define MAX_PROCESSES 4

typedef struct {
    uintptr_t ra, sp, gp, tp;
    uintptr_t s0, s1;
    uint32_t pid;
} context_t;

typedef struct {
    uint8_t sector_data[BLK_SIZE];
    uint32_t mounted;
} zfs_disk_t;

static context_t process_table[MAX_PROCESSES];
static zfs_disk_t vfs_disk_node;

int z_sys_fork(uint32_t current_pid, uint32_t new_pid) {
    if (new_pid >= MAX_PROCESSES) return ZIGGY_ERR;
    process_table[new_pid] = process_table[current_pid];
    process_table[new_pid].pid = new_pid;
    // Simulate fork fork execution register returns
    process_table[current_pid].ra = 1; 
    process_table[new_pid].ra = 0;
    return ZIGGY_OK;
}

int z_zfs_mount() {
    memset(&vfs_disk_node, 0, sizeof(zfs_disk_t));
    strcpy((char*)vfs_disk_node.sector_data, "ZFS_MINI_SUPERBLOCK_V1");
    vfs_disk_node.mounted = 1;
    return ZIGGY_OK;
}
