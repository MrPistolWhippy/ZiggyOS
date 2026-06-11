#include <stdint.h>
#include <stddef.h>

#define MAX_TASKS 2
#define STACK_SIZE 1024

typedef struct { uint32_t esp; uint32_t id; uint32_t stack[STACK_SIZE]; } task_control_block_t;
static task_control_block_t tcb_list[MAX_TASKS];
static int current_task_index = 0;

uint32_t system_ticks = 0;
uint32_t calc_counter = 0; // Dynamic calculations metric accumulator

extern int is_vga_mode, win_visible, win_x, win_y;
extern void draw_string(const char* s, int x, int y, uint8_t c);

uint32_t schedule_preempt(uint32_t current_esp) {
    system_ticks++;
    if (is_vga_mode && (system_ticks % 100 == 0)) {
        uint32_t tot = system_ticks / 100;
        char ts[9] = {'0'+(tot/60)/10, '0'+(tot/60)%10, ':', '0'+(tot%60)/10, '0'+(tot%60)%10, ' ', 'J', 'A', '\0'};
        draw_string(ts, 240, 186, 14);
        
        // Dynamically compute and update our calculations metrics if the terminal app window is open
        if (win_visible) {
            calc_counter += 7; // Perform rapid addition scaling calculation tasks
            char calc_str[10];
            calc_str[0] = 'V'; calc_str[1] = 'A'; calc_str[2] = 'L'; calc_str[3] = ':'; calc_str[4] = ' ';
            calc_str[5] = '0' + ((calc_counter / 1000) % 10);
            calc_str[6] = '0' + ((calc_counter / 100) % 10);
            calc_str[7] = '0' + ((calc_counter / 10) % 10);
            calc_str[8] = '0' + (calc_counter % 10);
            calc_str[9] = '\0';
            // Render calculations inside the inner terminal body window boundaries
            draw_string(calc_str, win_x + 10, win_y + 52, 10);
        }
    }
    tcb_list[current_task_index].esp = current_esp;
    current_task_index = (current_task_index + 1) % MAX_TASKS;
    return tcb_list[current_task_index].esp;
}

void create_kernel_thread(int id, void (*entry)(void)) {
    task_control_block_t* t = &tcb_list[id]; t->id = id;
    uint32_t* s = &t->stack[STACK_SIZE - 1];
    *(--s) = 0x00200202; *(--s) = 0x08; *(--s) = (uint32_t)entry;
    for (int i=0; i<8; i++) *(--s) = 0;
    t->esp = (uint32_t)s;
}
void init_multitasking(void (*t1)(void), void (*t2)(void)) {
    create_kernel_thread(0, t1); create_kernel_thread(1, t2); current_task_index = 0;
}
