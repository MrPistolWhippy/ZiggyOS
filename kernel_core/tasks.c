#include "io.h"

struct task_control_block {
int id;
unsigned int esp;
unsigned int eip;
int state; // 0 = ready, 1 = running
};

struct task_control_block task_queue[4];
int current_task_index = 0;
int total_tasks = 1; // Task 0 is always the main kernel entry loop

void init_scheduler() {
// Initialize task 0 as your active kernel shell process
task_queue[0].id = 0;
task_queue[0].state = 1;
task_queue[0].esp = 0;
task_queue[0].eip = 0;
}

void create_task(void (*entry_point)(), void* stack_space) {
if (total_tasks >= 4) return;

task_queue[total_tasks].id = total_tasks;
task_queue[total_tasks].state = 0;
// Point the execution stack to the top end of the reserved memory buffer
task_queue[total_tasks].esp = (unsigned int)stack_space + 4096;
task_queue[total_tasks].eip = (unsigned int)entry_point;

total_tasks++;
}

void task_switch() {
if (total_tasks <= 1) return; // Skip switching if no background tasks exist

task_queue[current_task_index].state = 0; // Suspend current task

// Hop to the next available task slot in round-robin fashion
current_task_index = (current_task_index + 1) % total_tasks;

task_queue[current_task_index].state = 1; // Mark next task as active

// Direct Assembly Context Switch (Simulated thread jump logic)
__asm__ __volatile__ (
"movl %0, %%esp \n\t"
"jmp *%1"
:
: "r"(task_queue[current_task_index].esp), "r"(task_queue[current_task_index].eip)
: "memory"
);
}

void pit_interrupt_scheduler_hook() {
if (total_tasks <= 1) {
// Send End-of-Interrupt (EOI) command signal byte to master PIC chip port 0x20
outb(0x20, 0x20);
return;
}

task_queue[current_task_index].state = 0;
current_task_index = (current_task_index + 1) % total_tasks;
task_queue[current_task_index].state = 1;

outb(0x20, 0x20); // Acknowledge the hardware clock interrupt line

// Context switch assembly stub
__asm__ __volatile__ (
"movl %0, %%esp \n\t"
"jmp *%1"
:
: "r"(task_queue[current_task_index].esp), "r"(task_queue[current_task_index].eip)
: "memory"
);
}
