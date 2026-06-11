#include "io.h"
struct tcb_struct { int id; unsigned int esp; unsigned int eip; int state; };
struct tcb_struct tasks[4];
int current_task = 0; int max_tasks = 1;
void init_scheduler() { tasks[0].id = 0; tasks[0].state = 1; tasks[0].esp = 0; tasks[0].eip = 0; }
void create_task(void (*ep)(), void* stack) {
    if (max_tasks >= 4) return;
    tasks[max_tasks].id = max_tasks; tasks[max_tasks].state = 0;
    tasks[max_tasks].esp = (unsigned int)stack + 4096; tasks[max_tasks].eip = (unsigned int)ep;
    max_tasks++;
}
void task_switch() {
    if (max_tasks <= 1) return;
    tasks[current_task].state = 0; current_task = (current_task + 1) % max_tasks; tasks[current_task].state = 1;
    __asm__ __volatile__ ("movl %0, %%esp \n\t jmp *%1" : : "r"(tasks[current_task].esp), "r"(tasks[current_task].eip) : "memory");
}
void pit_interrupt_scheduler_hook() {
    if (max_tasks <= 1) { outb(0x20, 0x20); return; }
    tasks[current_task].state = 0; current_task = (current_task + 1) % max_tasks; tasks[current_task].state = 1;
    outb(0x20, 0x20);
    __asm__ __volatile__ ("movl %0, %%esp \n\t jmp *%1" : : "r"(tasks[current_task].esp), "r"(tasks[current_task].eip) : "memory");
}
void print_process_status() {
    print("PID  |  Status   |  Memory Address\n---------------------------------\n");
    for (int i = 0; i < max_tasks; i++) {
        print(" "); print_char('0' + tasks[i].id);
        print(tasks[i].state == 1 ? "   |  RUNNING  |  0x" : "   |  READY    |  0x");
        unsigned int addr = tasks[i].eip; char buf[16]; int idx = 0;
        if (addr == 0) { print("00000000\n"); continue; }
        while (addr > 0) { unsigned int rem = addr % 16; buf[idx++] = (rem < 10) ? '0' + rem : 'A' + (rem - 10); addr /= 16; }
        while (idx > 0) print_char(buf[--idx]); print("\n");
    }
}
char mailbox[128]; int msg_ready = 0;
void ipc_send(const char* message) {
    int i = 0; while (message[i] != '\0' && i < 127) { mailbox[i] = message[i]; i++; }
    mailbox[i] = '\0'; msg_ready = 1;
}
void ipc_receive() {
    if (msg_ready == 1) { print("Mailbox Message: "); print(mailbox); print("\n"); msg_ready = 0; }
    else { print("Mailbox Empty.\n"); }
}
void background_worker_task() {
    volatile unsigned short* vram = (volatile unsigned short*)0xB8000;
    unsigned char spin[4] = {'|', '/', '-', ''}; int i = 0;
    while(1) { vram[79] = (0x0E << 8) | spin[i]; i = (i + 1) % 4; for(volatile int d=0; d<1000000; d++); }
}
unsigned int target_hash = 208947265;
unsigned int app_calculate_string_hash(const char* str) {
    unsigned int hash = 5381; int i = 0;
    while (str[i] != '\0') { hash = ((hash << 5) + hash) + str[i]; i++; }
    return hash;
}
int app_verify_system_login(const char* pass) {
    if (app_calculate_string_hash(pass) == target_hash) { print("\nAuthentication Successful!\n"); return 1; }
    print("\nAccess Denied!\n"); return 0;
}
unsigned int seed = 12345;
unsigned int quick_rand() { seed = (seed * 1103515245 + 12345) & 0x7FFFFFFF; return seed; }
void app_matrix_screensaver() {
    volatile unsigned short* vram = (volatile unsigned short*)0xB8000;
    for(int i = 0; i < 2000; i++) vram[i] = (0x0F << 8) | ' ';
    print("Press any key to exit Matrix mode...\n");
    while(!keyboard_has_key()) {
        int x = quick_rand() % 80; int y = quick_rand() % 25; char c = 33 + (quick_rand() % 93);
        vram[y * 80 + x] = (0x0A << 8) | c; for(volatile int d = 0; d < 10000; d++);
    }
    shell_clear();
}
void app_run_calculator(const char* expr) {
    int num1 = 0, num2 = 0, idx = 0; char op = 0;
    while(expr[idx] == ' ' || (expr[idx] >= 'a' && expr[idx] <= 'z')) idx++;
    while(expr[idx] >= '0' && expr[idx] <= '9') { num1 = num1 * 10 + (expr[idx] - '0'); idx++; }
    while(expr[idx] == ' ') idx++;
    if(expr[idx] == '+' || expr[idx] == '-' || expr[idx] == '*' || expr[idx] == '/') { op = expr[idx]; idx++; }
    while(expr[idx] == ' ') idx++;
    while(expr[idx] >= '0' && expr[idx] <= '9') { num2 = num2 * 10 + (expr[idx] - '0'); idx++; }
    print("\nResult: "); int res = 0;
    if(op == '+') res = num1 + num2; else if(op == '-') res = num1 - num2; else if(op == '*') res = num1 * num2;
    else if(op == '/') { if(num2 == 0) { print("Error: Div by 0!\n"); return; } res = num1 / num2; }
    char buf[16]; int i = 0; if(res == 0) { print_char('0'); print("\n"); return; }
    while(res > 0) { buf[i++] = '0' + (res % 10); res /= 10; }
    while(i > 0) print_char(buf[--i]); print("\n");
}
void app_print_system_info() { print("\n========================================\n       ZIGGYOS HARDWARE INSPECTOR       \n========================================\n"); }
void app_play_snake() {
    volatile unsigned short* vram = (volatile unsigned short*)0xB8000; shell_clear();
    int sx[100]; int sy[100]; int len = 3; int dx = 1; int dy = 0;
    for(int i = 0; i < len; i++) { sx[i] = 10 - i; sy[i] = 10; }
    int fx = 20; int fy = 15; int over = 0;
    print("SNAKE GAME - WASD keys to move. Q to quit.\n");
    while(!over) {
        for(int i = 80; i < 2000; i++) vram[i] = (0x0F << 8) | ' ';
        vram[fy * 80 + fx] = (0x0B << 8) | 'O';
        if(keyboard_has_key()) {
            unsigned char code = keyboard_read(); char key = scancode_to_ascii(code);
            if(key == 'w' && dy == 0) { dx = 0; dy = -1; }
            else if(key == 's' && dy == 0) { dx = 0; dy = 1; }
            else if(key == 'a' && dx == 0) { dx = -1; dy = 0; }
            else if(key == 'd' && dx == 0) { dx = 1; dy = 0; }
            else if(key == 'q') break;
        }
        for(int i = len - 1; i > 0; i--) { sx[i] = sx[i-1]; sy[i] = sy[i-1]; }
        sx[0] += dx; sy[0] += dy;
        if(sx[0] < 0 || sx[0] > 79 || sy[0] < 1 || sy[0] > 24) over = 1;
        if(sx[0] == fx && sy[0] == fy) {
            if(len < 100) len++; fx = 2 + (quick_rand() % 75); fy = 2 + (quick_rand() % 20);
            play_sound(880); for(volatile int d=0; d<100000; d++); nosound();
        }
        for(int i = 0; i < len; i++) vram[sy[i] * 80 + sx[i]] = (0x0A << 8) | 'S';
        for(volatile int d = 0; d < 3000000; d++);
    }
    shell_clear(); print("Game Over!\n-> ");
}
void app_set_system_theme(char selection) {
    extern char current_color;
    if(selection == 't') current_color = 0x03;
    else if(selection == 'e') current_color = 0x0A;
    else if(selection == 'a') current_color = 0x06;
}
