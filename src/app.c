#include "io.h"

struct task_control_block { int id; unsigned int esp; unsigned int eip; int state; };
struct task_control_block task_queue[4];
int current_task_index = 0;
int total_tasks = 1;

void init_scheduler() {
    task_queue[0].id = 0; task_queue[0].state = 1;
    task_queue[0].esp = 0; task_queue[0].eip = 0;
}

void create_task(void (*entry_point)(), void* stack_space) {
    if (total_tasks >= 4) return;
    task_queue[total_tasks].id = total_tasks;
    task_queue[total_tasks].state = 0;
    task_queue[total_tasks].esp = (unsigned int)stack_space + 4096;
    task_queue[total_tasks].eip = (unsigned int)entry_point;
    total_tasks++;
}

void task_switch() {
    if (total_tasks <= 1) return;
    task_queue[current_task_index].state = 0;
    current_task_index = (current_task_index + 1) 
void print_process_status() {
    extern void print(const char* str);
    extern void print_char(char c);
    print("PID  |  Status   |  Memory Addressn---------------------------------n");
    for (int i = 0; i < total_tasks; i++) {
        print(" "); print_char('0' + task_queue[i].id);
        print(task_queue[i].state == 1 ? "   |  RUNNING  |  0x" : "   |  READY    |  0x");
        unsigned int addr = task_queue[i].eip;
        char buf[16]; int idx = 0;
        if (addr == 0) { print("00000000n"); continue; }
        while (addr > 0) { unsigned int rem = addr % 16; buf[idx++] = (rem < 10) ? '0' + rem : 'A' + (rem - 10); addr /= 16; }
        while (idx > 0) print_char(buf[--idx]);
        print("n");
    }
}

char ipc_mailbox[128];
int ipc_has_message = 0;

void ipc_send(const char* message) {
    int i = 0; while (message[i] != '0' && i < 127) { ipc_mailbox[i] = message[i]; i++; }
    ipc_mailbox[i] = '0'; ipc_has_message = 1;
}

void ipc_receive() {
    extern void print(const char* str);
    if (ipc_has_message == 1) { print("Mailbox Message: "); print(ipc_mailbox); print("n"); ipc_has_message = 0; }
    else { print("Mailbox Empty.n"); }
}

void background_worker_task() {
    volatile unsigned short* vram = (volatile unsigned short*)0xB8000;
    unsigned char spin[4] = {'|', '/', '-', ''};
    int i = 0;
    while(1) {
        vram[79] = (0x0E << 8) | spin[i];
        i = (i + 1) % 4;
        for(volatile int d=0; d<1000000; d++);
    }
}

unsigned int target_password_hash = 208947265;
unsigned int app_calculate_string_hash(const char* str) {
    unsigned int hash = 5381; int i = 0;
    while (str[i] != '0') { hash = ((hash << 5) + hash) + str[i]; i++; }
    return hash;
}

int app_verify_system_login(const char* input_password) {
    extern void print(const char* str);
    unsigned int input_hash = app_calculate_string_hash(input_password);
    if (input_hash == target_password_hash) { print("nAuthentication Successful!n"); return 1; }
    print("nAccess Denied!n"); return 0;
}

unsigned int rand_seed = 12345;
unsigned int quick_rand() { rand_seed = (rand_seed * 1103515245 + 12345) \& 0x7FFFFFFF; return rand_seed; }

void app_matrix_screensaver() {
    extern void print(const char* str); extern void shell_clear(); extern int keyboard_has_key();
    volatile unsigned short* vram = (volatile unsigned short*)0xB8000;
    for(int i = 0; i < 2000; i++) vram[i] = (0x0F << 8) | ' ';
    print("Press any key to exit Matrix mode...n");
    while(!keyboard_has_key()) {
        int x = quick_rand() % 80; int y = quick_rand() % 25; char c = 33 + (quick_rand() % 93);
        vram[y * 80 + x] = (0x0A << 8) | c; for(volatile int d = 0; d < 10000; d++);
    }
    shell_clear();
}

void app_run_calculator(const char* expr) {
    extern void print(const char* str); extern void print_char(char c);
    int num1 = 0, num2 = 0, idx = 0; char op = 0;
    while(expr[idx] == ' ' || (expr[idx] >= 'a' && expr[idx] <= 'z')) idx++;
    while(expr[idx] >= '0' && expr[idx] <= '9') { num1 = num1 * 10 + (expr[idx] - '0'); idx++; }
    while(expr[idx] == ' ') idx++;
    if(expr[idx] == '+' || expr[idx] == '-' || expr[idx] == '*' || expr[idx] == '/') { op = expr[idx]; idx++; }
    while(expr[idx] == ' ') idx++;
    while(expr[idx] >= '0' && expr[idx] <= '9') { num2 = num2 * 10 + (expr[idx] - '0'); idx++; }
    print("nResult: "); int res = 0;
    if(op == '+') res = num1 + num2; else if(op == '-') res = num1 - num2; else if(op == '*') res = num1 * num2;
    else if(op == '/') { if(num2 == 0) { print("Error: Div by 0!n"); return; } res = num1 / num2; }
    char buf[16]; int i = 0; if(res == 0) { print_char('0'); print("n"); return; }
    while(res > 0) { buf[i++] = '0' + (res % 10); res /= 10; }
    while(i > 0) print_char(buf[--i]);
    print("n");
}
// Build Trigger Stamp: Thu Jun 11 04:57:11 UTC 2026
