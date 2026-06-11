#include "io.h"

unsigned char keyboard_read();
int keyboard_has_key();
char scancode_to_ascii(unsigned char scancode);
void print(const char* str);
void print_char(char c);
void shell_backspace();
void shell_clear();
void init_gdt();
void init_idt();
void idt_set_gate(unsigned char num, unsigned long base, unsigned short sel, unsigned char flags);
void pic_remap();
void init_mouse();
void mouse_poll_movement();
void init_scheduler();
void create_task(void (*ep)(), void* stack);
void init_ramfs();

unsigned long timer_ticks = 0;
char cmd_buffer[256];
int cmd_index = 0;

void print_uptime() {
unsigned long seconds = timer_ticks / 10000;
print("System Uptime: ");
char buf[16];
int i = 0;
if(seconds == 0) { print("0s\n"); return; }
while(seconds > 0) {
buf[i++] = '0' + (seconds % 10);
seconds /= 10;
}
while(i > 0) { print_char(buf[--i]); }
print("s\n");
}

void process_command() {
print("\n");
if (cmd_buffer[0] == 'r' && cmd_buffer[1] == 'e' && cmd_buffer[2] == 'b' && cmd_buffer[3] == 'o' && cmd_buffer[4] == 'o' && cmd_buffer[5] == 't') {
print("Rebooting...\n");
outb(0x64, 0xFE);
} else if (cmd_buffer[0] == 's' && cmd_buffer[1] == 'h' && cmd_buffer[2] == 'u' && cmd_buffer[3] == 't') {
print("Powering down system...\n");
outw(0xB004, 0x2000); outw(0x604, 0x2000);
while(1);
} else if (cmd_buffer[0] == 'p' && cmd_buffer[1] == 's') {
extern void print_process_status();
print_process_status();
print("-> ");
} else if (cmd_buffer[0] == 'm' && cmd_buffer[1] == 's' && cmd_buffer[2] == 'g') {
extern void ipc_send(const char* m);
extern void ipc_receive();
ipc_send("Hello from Task 0!");
ipc_receive();
} else if (cmd_buffer[0] == 'b' && cmd_buffer[1] == 'e' && cmd_buffer[2] == 'e' && cmd_buffer[3] == 'p') {
extern void mmu_trigger_beep();
print("Sounding hardware speaker...\n");
mmu_trigger_beep();
} else if (cmd_buffer[0] == 'x' && cmd_buffer[1] == 'x' && cmd_buffer[2] == 'd') {
extern void mmu_hex_dump(void* start);
extern struct gdt_entry gdt;
mmu_hex_dump(&gdt);
} else if (cmd_buffer[0] == 'm' && cmd_buffer[1] == 'a') {
extern void app_matrix_screensaver();
app_matrix_screensaver();
} else if (cmd_buffer[0] == 'i' && cmd_buffer[1] == 'n') {
extern void app_print_system_info();
app_print_system_info();
} else if (cmd_buffer[0] == 'l' && cmd_buffer[1] == 'o' && cmd_buffer[2] == 'g') {
extern int app_verify_system_login(const char* p);
print("\nEnter Password: ");
app_verify_system_login("ziggy");
} else if (cmd_buffer[0] == 'c' && cmd_buffer[1] == 'a' && cmd_buffer[2] == 'l') {
extern void app_run_calculator(const char* e);
app_run_calculator(&cmd_buffer[5]);
} else {
print("Unknown command.\n-> ");
}
}

char task_stack[4096];
void background_worker_task();

void kernel_main() {
init_gdt();
init_idt();
pic_remap();
init_mouse();
init_scheduler();
init_ramfs();
create_task(background_worker_task, task_stack);
shell_clear();
__asm__ __volatile__("sti");
print("ZiggyOS Storage Enabled Shell\n-> ");
while(1) {
timer_ticks++;
mouse_poll_movement();
if (keyboard_has_key()) {
unsigned char scancode = keyboard_read();
char ascii = scancode_to_ascii(scancode);
if (ascii != 0) {
if (ascii == '\n') {
cmd_buffer[cmd_index] = '\0';
process_command();
cmd_index = 0;
} else if (ascii == '\b') {
if (cmd_index > 0) { cmd_index--; shell_backspace(); }
} else {
if (cmd_index < 255) { cmd_buffer[cmd_index++] = ascii; print_char(ascii); }
}
}
}
}
}
