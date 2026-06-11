#include "io.h"

// 1. COOPERATIVE ROUND-ROBIN TASK SCHEDULER & CONTEXT MANAGER
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
    current_task_index = (current_task_index + 1) % total_tasks;
    task_queue[current_task_index].state = 1;
    __asm__ __volatile__ ("movl %0, %%esp \n\t jmp *%1" : : "r"(task_queue[current_task_index].esp), "r"(task_queue[current_task_index].eip) : "memory");
}

void pit_interrupt_scheduler_hook() {
    if (total_tasks <= 1) { outb(0x20, 0x20); return; }
    task_queue[current_task_index].state = 0;
    current_task_index = (current_task_index + 1) % total_tasks;
    task_queue[current_task_index].state = 1;
    outb(0x20, 0x20);
    __asm__ __volatile__ ("movl %0, %%esp \n\t jmp *%1" : : "r"(task_queue[current_task_index].esp), "r"(task_queue[current_task_index].eip) : "memory");
}

void print_process_status() {
    extern void print(const char* str);
    extern void print_char(char c);
    print("PID  |  Status   |  Memory Address\n---------------------------------\n");
    for (int i = 0; i < total_tasks; i++) {
        print(" "); print_char('0' + task_queue[i].id);
        print(task_queue[i].state == 1 ? "   |  RUNNING  |  0x" : "   |  READY    |  0x");
        unsigned int addr = task_queue[i].eip;
        char buf[16]; int idx = 0;
        if (addr == 0) { print("00000000\n"); continue; }
        while (addr > 0) {
            unsigned int rem = addr % 16;
            buf[idx++] = (rem < 10) ? '0' + rem : 'A' + (rem - 10);
            addr /= 16;
        }
        while (idx > 0) print_char(buf[--idx]);
        print("\n");
    }
}

// 2. KERNEL-SPACE INTER-PROCESS COMMUNICATION MAILBOX
char ipc_mailbox[128];
int ipc_has_message = 0;

void ipc_send(const char* message) {
    int i = 0;
    while (message[i] != '\0' && i < 127) { ipc_mailbox[i] = message[i]; i++; }
    ipc_mailbox[i] = '\0'; ipc_has_message = 1;
}

void ipc_receive() {
    extern void print(const char* str);
    if (ipc_has_message == 1) {
        print("Mailbox Message: "); print(ipc_mailbox); print("\n");
        ipc_has_message = 0;
    } else {
        print("Mailbox Empty.\n");
    }
}

// 3. BACKGROUND SPINNING ASSET WORKER THREAD
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

// 4. MULTI-USER PROFILE RECORDS DATABASE
struct user_profile { char username[16]; unsigned int password_hash; int active; };
struct user_profile system_users[5];
int total_system_users = 0;

unsigned int app_calculate_string_hash(const char* str) {
    unsigned int hash = 5381; int i = 0;
    while (str[i] != '\0') { hash = ((hash << 5) + hash) + str[i]; i++; }
    return hash;
}

void app_init_users() {
    system_users[0].active = 1; system_users[0].password_hash = 208947265; // "ziggy" prime code hash
    unsigned char* adm = (unsigned char*)"admin";
    for(int i=0; adm[i]!='\0'; i++) system_users[0].username[i] = adm[i];
    total_system_users = 1;
}

void app_user_add(const char* name, const char* raw_pass) {
    extern void print(const char* str);
    if (total_system_users >= 5) { print("Error: Max users reached!\n"); return; }
    int slot = total_system_users;
    system_usecat << 'EOF' >> src/app.c
void app_crypt_file() {
struct file_struct { char name[32]; char content[128]; int size; int active; };
extern struct file_struct ramfs[10];
if(ramfs[0].active == 1) {
char key = (char)(target_password_hash & 0xFF);
for(int i = 0; ramfs[0].content[i] != '\0' && i < 128; i++) ramfs[0].content[i] ^= key;
print("\nCipher process complete.\n");
}
}
void app_run_calculator(const char* expr) {
extern void print_char(char c);
int num1 = 0, num2 = 0, idx = 0; char op = 0;
while(expr[idx] == ' ' || (expr[idx] >= 'a' && expr[idx] <= 'z')) idx++;
while(expr[idx] >= '0' && expr[idx] <= '9') { num1 = num1 * 10 + (expr[idx] - '0'); idx++; }
while(expr[idx] == ' ') idx++;
if(expr[idx] == '+' || expr[idx] == '-' || expr[idx] == '*' || expr[idx] == '/') { op = expr[idx]; idx++; }
while(expr[idx] == ' ') idx++;
while(expr[idx] >= '0' && expr[idx] <= '9') { num2 = num2 * 10 + (expr[idx] - '0'); idx++; }
print("\nResult: "); int res = 0;
if(op == '+') res = num1 + num2;
else if(op == '-') res = num1 - num2;
else if(op == '*') res = num1 * num2;
else if(op == '/') { if(num2 == 0) { print("Error: Div by 0!\n"); return; } res = num1 / num2; }
char buf[16]; int i = 0;
if(res == 0) { print_char('0'); print("\n"); return; }
while(res > 0) { buf[i++] = '0' + (res % 10); res /= 10; }
while(i > 0) print_char(buf[--i]);
print("\n");
}
void app_play_snake() {
extern unsigned char keyboard_read(); extern char scancode_to_ascii(unsigned char scancode);
extern void play_sound(unsigned int freq); extern void nosound(); extern void shell_clear(); extern int keyboard_has_key();
volatile unsigned short* vram = (volatile unsigned short*)0xB8000;
shell_clear();
int snake_x[100]; int snake_y[100]; int snake_length = 3; int dir_x = 1; int dir_y = 0;
for(int i = 0; i < snake_length; i++) { snake_x[i] = 10 - i; snake_y[i] = 10; }
int food_x = 20; int food_y = 15; int game_over = 0;
print("SNAKE GAME - WASD keys to move. Q to quit.\n");
while(!game_over) {
for(int i = 80; i < 2000; i++) vram[i] = (0x0F << 8) | ' ';
vram[food_y * 80 + food_x] = (0x0B << 8) | 'O';
if(keyboard_has_key()) {
unsigned char code = keyboard_read(); char key = scancode_to_ascii(code);
if(key == 'w' && dir_y == 0) { dir_x = 0; dir_y = -1; }
else if(key == 's' && dir_y == 0) { dir_x = 0; dir_y = 1; }
else if(key == 'a' && dir_x == 0) { dir_x = -1; dir_y = 0; }
else if(key == 'd' && dir_x == 0) { dir_x = 1; dir_y = 0; }
else if(key == 'q') break;
}
for(int i = snake_length - 1; i > 0; i--) { snake_x[i] = snake_x[i-1]; snake_y[i] = snake_y[i-1]; }
snake_x[0] += dir_x; snake_y[0] += dir_y;
if(snake_x[0] < 0 || snake_x[0] > 79 || snake_y[0] < 1 || snake_y[0] > 24) game_over = 1;
if(snake_x[0] == food_x && snake_y[0] == food_y) {
if(snake_length < 100) snake_length++;
food_x = 2 + (quick_rand() % 75); food_y = 2 + (quick_rand() % 20);
play_sound(880); for(volatile int d=0; d<100000; d++); nosound();
}
for(int i = 0; i < snake_length; i++) vram[snake_y[i] * 80 + snake_x[i]] = (0x0A << 8) | 'S';
for(volatile int d = 0; d < 3000000; d++) ;
}
shell_clear(); print("Game Over!\n-> ");
}
