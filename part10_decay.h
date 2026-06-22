#include "part9_core.h"

typedef struct {
    uint32_t base_priority;
    uint32_t active_priority;
    uint32_t cpu_cycles_consumed;
} decay_thread_t;

static decay_thread_t run_queue[MAX_PROCESSES];

void z_decay_init() {
    for (int i = 0; i < MAX_PROCESSES; i++) {
        run_queue[i].base_priority = (i + 1) * 10;
        run_queue[i].active_priority = run_queue[i].base_priority;
        run_queue[i].cpu_cycles_consumed = 0;
    }
}

void z_decay_process_tick() {
    for (int i = 0; i < MAX_PROCESSES; i++) {
        if (run_queue[i].cpu_cycles_consumed > 5) {
            // Anti-starvation priority decay dynamic step
            if (run_queue[i].active_priority > 2) {
                run_queue[i].active_priority -= 2;
            }
        } else {
            // Boost long-waiting threads to balance resource ingestion
            run_queue[i].active_priority += 1;
        }
    }
}
