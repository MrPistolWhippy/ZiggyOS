#include "io.h"

void init_scheduler() {}
void create_task(void (*ep)(), void* stack) { (void)ep; (void)stack; }
void task_switch() {}
void pit_interrupt_scheduler_hook() {}

void print_process_status() {
    print("PID 0: RUNNING\n");
}

char mailbox[128];
int msg_ready = 0;

void ipc_send(const char* message) {
    int i = 0; while (message[i] != '\0' && i < 127) { mailbox[i] = message[i]; i++; }
    mailbox[i] = '\0'; msg_ready = 1;
}

void ipc_receive() {
    if (msg_ready) print(mailbox);
}

void background_worker_task() {}
void app_matrix_screensaver() {}
void app_run_calculator(const char* expr) { (void)expr; }
void app_print_system_info() {}
void app_play_snake() {}
void app_set_system_theme(char selection) { (void)selection; }

int app_verify_system_login(const char* p) {
    (void)p;
    return 1;
}
