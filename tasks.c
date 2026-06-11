#include <stdint.h>
typedef struct { uint32_t *sp; uint32_t id; uint32_t state; } TCB_t;
TCB_t tasks[4]; uint32_t current_id = 0;
void yield() { uint32_t next = (current_id + 1) % 4; current_id = next; }

void print_process_status() {
print("PID  |  Status   |  Memory Address\n");
print("---------------------------------\n");
for (int i = 0; i < total_tasks; i++) {
print(" ");
print_char('0' + task_queue[i].id);
if (task_queue[i].state == 1) {
print("   |  RUNNING  |  0x");
} else {
print("   |  READY    |  0x");
}

unsigned int addr = task_queue[i].eip;
char buf[16];
int idx = 0;
if (addr == 0) { print("00000000\n"); continue; }
while (addr > 0) {
unsigned int rem = addr % 16;
if (rem < 10) buf[idx++] = '0' + rem;
else buf[idx++] = 'A' + (rem - 10);
addr /= 16;
}
while (idx > 0) print_char(buf[--idx]);
print("\n");
}
}
