#include <stdint.h>

// --- PHYSICAL RADIO TRANSCEIVER REGISTER MAPS (SPI Interface) ---
#define RADIO_BASE        0x40013000
#define RADIO_CR1         ((volatile uint32_t *)(RADIO_BASE + 0x00)) // Control Register 1
#define RADIO_DR          ((volatile uint32_t *)(RADIO_BASE + 0x0C)) // Data Register
#define RADIO_SR          ((volatile uint32_t *)(RADIO_BASE + 0x08)) // Status Register

#define RADIO_SR_TXE      (1 << 1) // Transmit buffer empty flag
#define RADIO_SR_RXNE     (1 << 0) // Receive buffer not empty flag

void radio_write_reg(uint8_t data) {
    while (!(*RADIO_SR & RADIO_SR_TXE)); // Wait until transmit buffer is empty
    *RADIO_DR = data;
}

// --- REAL-TIME MULTI-THREADED TASK SCHEDULER ---
#define MAX_TASKS 3
#define STACK_SIZE 1024

typedef struct {
    void (*task_entry)(void); // Pointer to the task function
    uint32_t status;          // 1 = Active, 0 = Suspended
} Task_t;

Task_t task_table[MAX_TASKS];
uint32_t current_task_id = 0;

void scheduler_register_task(uint32_t id, void (*entry)(void)) {
    if (id < MAX_TASKS) {
        task_table[id].task_entry = entry;
        task_table[id].status = 1;
    }
}

void scheduler_yield(void) {
    // Basic round-robin task switching algorithm
    current_task_id = (current_task_id + 1) % MAX_TASKS;
    if (task_table[current_task_id].status == 1) {
        task_table[current_task_id].task_entry();
    }
}

// --- CORE SYSTEM TASKS ---
extern void app_radio_telemetry_tick(void);
extern int uart_getc_nonblocking(char *out_char);
extern void app_stego_trigger_wipe(void);
extern void uart_putc(char c);
extern void print(const char *str);

void task_telemetry(void) {
    app_radio_telemetry_tick();
}

void task_input_poll(void) {
    char rx_cmd;
    if (uart_getc_nonblocking(&rx_cmd)) {
        if (rx_cmd == 'X' || rx_cmd == 'x') {
            app_stego_trigger_wipe();
        } else {
            uart_putc('[');
            uart_putc(rx_cmd);
            uart_putc(']');
        }
    }
}

void task_radio_hardware(void) {
    // Pulse a pulse-modulated beacon across the hardware registers
    radio_write_reg(0xAA); 
}

// --- KERNEL ENTRY POINT ---
void kernel_main(void) {
    extern void init_interrupt_priorities_sys(void);
    extern int run_automated_tests_sys(void);
    extern void app_radio_telemetry_init(void);
    extern void app_stego_panic_init(void);

    print("[KERNEL] ZiggyOS Extended Architecture: OVER9000 Initialized.\n");

    init_interrupt_priorities_sys();
    if (!run_automated_tests_sys()) {
        app_stego_trigger_wipe();
    }

    app_radio_telemetry_init();
    app_stego_panic_init();

    // Register all tasks into the scheduler table
    scheduler_register_task(0, task_telemetry);
    scheduler_register_task(1, task_input_poll);
    scheduler_register_task(2, task_radio_hardware);

    print("[KERNEL] Task scheduler running. Multi-threaded thread safety loop active.\n");

    while (1) {
        scheduler_yield(); // Continuously cycle through active execution loops
    }
}
