#include <stdint.h>
#include "/root/workspace/src/ziggyos.h"

typedef struct { uint32_t *sp; uint32_t id; uint32_t state; uint32_t eip; } TCB_t;
TCB_t tasks[4] = {
    {0, 0, 1, 0x100000},
    {0, 1, 0, 0x102000},
    {0, 2, 0, 0x104000},
    {0, 3, 0, 0x106000}
};
uint32_t current_id = 0;
int total_tasks = 4;

void yield() { current_id = (current_id + 1) % 4; }

void print_process_status() {
    print("PID  |  Status   |  Memory Address\n");
    print("---------------------------------\n");
    for (int i = 0; i < total_tasks; i++) {
        print(" ");
        putc('0' + tasks[i].id);
        if (tasks[i].state == 1) print("   |  RUNNING  |  0x");
        else print("   |  READY    |  0x");

        unsigned int addr = tasks[i].eip;
        char buf[16];
        int idx = 0;
        if (addr == 0) { print("00000000\n"); continue; }
        while (addr > 0) {
            unsigned int rem = addr % 16;
            if (rem < 10) buf[idx++] = '0' + rem;
            else buf[idx++] = 'A' + (rem - 10);
            addr /= 16;
        }
        while (idx > 0) putc(buf[--idx]);
        print("\n");
    }
}

char ipc_mailbox[128];
int ipc_has_message = 0;

void ipc_send(const char* message) {
    int i = 0;
    while (message[i] != '\0' && i < 127) {
        ipc_mailbox[i] = message[i];
        i++;
    }
    ipc_mailbox[i] = '\0';
    ipc_has_message = 1;
}

void ipc_receive() {
    if (ipc_has_message == 1) {
        print("Mailbox Message: ");
        print(ipc_mailbox);
        print("\n");
        ipc_has_message = 0;
    } else {
        print("Mailbox Empty.\n");
    }
}
