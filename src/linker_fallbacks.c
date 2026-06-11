#include <stdint.h>

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define MAX_TASKS 3

static uint16_t* const vga_buffer = (uint16_t*)0xB8000;
static int vga_column = 0;
static int vga_row = 0;
static uint8_t current_color = 0x07; // Light grey on black default

// Task Control Block (TCB) Structure
typedef struct {
    uint32_t esp; // Saved stack pointer
    uint32_t active;
} tcb_t;

static tcb_t task_table[MAX_TASKS];
static int current_task = 0;

// Set text foreground and background color attribute byte
void set_text_color(uint8_t fg, uint8_t bg) {
    current_color = fg | (bg << 4);
}

void print(const char* str) {
    for (int i = 0; str[i] != '\0'; i++) {
        char c = str[i];
        if (c == '\n') {
            vga_column = 0;
            vga_row++;
            continue;
        }
        int index = (vga_row * VGA_WIDTH) + vga_column;
        vga_buffer[index] = (uint16_t)c | (current_color << 8);
        vga_column++;
        if (vga_column >= VGA_WIDTH) {
            vga_column = 0;
            vga_row++;
        }
    }
}

// Basic cooperative scheduler context switch routine
void sys_yield(void) {
    int old_task = current_task;
    current_task = (current_task + 1) % MAX_TASKS;
    
    // Low-level assembly context switch wrapper swap
    asm volatile(
        "movl %%esp, %0\n\t"  // Save current task stack pointer
        "movl %1, %%esp\n\t"  // Load next task stack pointer
        : "=m"(task_table[old_task].esp)
        : "m"(task_table[current_task].esp)
        : "memory"
    );
}
