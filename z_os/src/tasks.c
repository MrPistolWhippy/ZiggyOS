#include <stdint.h>

#define MAX_TASKS 4
#define STACK_SIZE 1024

typedef struct {
    uint32_t esp;
    uint32_t id;
    uint32_t state;
    uint32_t stack[STACK_SIZE];
} task_control_block_t;

static task_control_block_t tcb_list[MAX_TASKS];
static int current_task_index = 0;

uint32_t system_ticks = 0;
uint32_t calc_counter = 0;

extern int is_vga_mode, win_visible, win_x, win_y;
extern void draw_string(const char* s, int x, int y, uint8_t c);

uint32_t schedule_preempt(uint32_t current_esp) {
    // 1. Save the context pointer of the current running task
    tcb_list[current_task_index].esp = current_esp;

    // 2. Increment system clock ticks
    system_ticks++;

    if (is_vga_mode && (system_ticks % 100 == 0)) {
        uint32_t t_tot = system_ticks / 100;
        char ts[9] = {'0'+(t_tot/60)/10, '0'+(t_tot/60)%10, ':', '0'+(t_tot%60)/10, '0'+(t_tot%60)%10, ':', ' ', '0', '\0'};
        draw_string(ts, 240, 186, 14);
    }

    if (win_visible) {
        calc_counter += 7;
    }

    // 3. Switch to the next task (Round Robin)
    current_task_index = (current_task_index + 1) % MAX_TASKS;

    // 4. Return the new target stack pointer for the assembly switcher to pop
    return tcb_list[current_task_index].esp;
}
