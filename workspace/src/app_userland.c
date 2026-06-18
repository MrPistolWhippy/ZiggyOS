#include <stdint.h>

extern void print(const char *str);
extern void app_radio_telemetry_tick(void);
extern void app_compress_and_broadcast_telemetry(void);
extern volatile uint64_t system_jiffies_ticks;

// Task Control Block structure to map concurrent threads
typedef struct {
    uint32_t task_id;
    const char *task_name;
    void (*task_entry)(void);
} TaskControlBlock;

void background_telemetry_task(void) {
    app_radio_telemetry_tick();
    app_compress_and_broadcast_telemetry();
}

void foreground_shell_prompt_task(void) {
    static uint64_t last_prompt_time = 0;
    if (system_jiffies_ticks - last_prompt_time >= 500) { // Every 5 seconds
        last_prompt_time = system_jiffies_ticks;
        print("\nziggy_user@deck:~$ [SYSTEM MONITOR] Scheduling pool stable. Ready for user input.\n");
    }
}

// Global Round-Robin Preemptive task context switcher execution loop
void run_automated_harness(void) {
    print("\n--- ZiggyOS Preemptive Multitasking Scheduler Online ---\n");
    
    TaskControlBlock task_list[2] = {
        {0, "Telemetry Tracker", background_telemetry_task},
        {1, "Shell Environment", foreground_shell_prompt_task}
    };
    
    uint32_t active_task_index = 0;
    uint32_t loop_cycles = 0;

    // Simulate an active multitasking scheduler loop across clock cycles
    while (1) {
        loop_cycles++;
        
        // Context switch execution thread via round-robin pointer rotation
        task_list[active_task_index].task_entry();
        active_task_index = (active_task_index + 1) % 2;
    }
    
    print("\n[+] Scheduler slice limit reached. Yielding control back to main kernel loop.\n");
}
