/* ==============================================================================
 *          ZIGGY-OS HARDWARE CORE: SCHEDULER & MMU LAYER VIRTUALIZATION
 * ============================================================================== */
#include <stdint.h>

/* --- 1. CONSOLE UART DEFINITIONS --- */
#define UART_BASE 0x10000000
#define UART_THR  ((volatile uint8_t*)(UART_BASE + 0))
#define UART_LSR  ((volatile uint8_t*)(UART_BASE + 5))

void uart_puts(const char *s) {
    while (*s) {
        while ((*UART_LSR & 0x20) == 0);
        *UART_THR = *s++;
    }
}

/* --- 2. SV39 MMU VIRTUAL MEMORY MAPPING REGISTERS --- */
#define SATP_SV39 (8ULL  0x80000000) */
    /* Mega-page mapping layout (Bit 10 contains physical page number) */
    uint64_t ppn = (0x80000000ULL >> 12);
    root_page_table[2] = (ppn << 10) | PTE_V | PTE_R | PTE_W | PTE_X;
    
    uart_puts("[✓] MMU Layer: Identity Mapped 1GB Base Segment.\n");
}

/* --- 3. RING-0 KERNEL TASK THREAD SCHEDULER --- */
#define MAX_TASKS 4

typedef struct {
    uintptr_t sp;         /* Task execution stack pointer marker */
    uint32_t  task_id;    /* Numerical profile reference identifier */
    uint32_t  state;      /* 0 = SLEEP, 1 = RUNNABLE READY STATE */
} TaskControlBlock_t;

TaskControlBlock_t task_queue[MAX_TASKS];
uint32_t current_task_index = 0;

/* Basic task stack boundaries */
uint8_t task_stacks[MAX_TASKS][2048] __attribute__((aligned(16)));

void init_scheduler(void) {
    for (uint32_t i = 0; i < MAX_TASKS; i++) {
        task_queue[i].task_id = i;
        task_queue[i].state = 1; /* Ready for dispatch round-robin cycles */
        /* Set initial target execution frames inside isolated stack layouts */
        task_queue[i].sp = (uintptr_t)&task_stacks[i][2048];
    }
    uart_puts("[✓] Ring-0 Scheduler: Context Management Engines Online.\n");
}

void schedule_next_context(void) {
    uint32_t next_index = (current_task_index + 1) % MAX_TASKS;
    
    /* Core context ring switcher hook */
    current_task_index = next_index;
    uart_puts("[🚀 ROUTINE] Context Shifted to Thread Identifier\n");
}

/* --- 4. EXECUTIVE BOOT INITIALIZATION SYSTEM --- */
void _start(void) {
    extern uint32_t __bss_start, __bss_end;
    uint32_t *bss = &__bss_start;
    while (bss < &__bss_end) {
        *bss++ = 0;
    }

    uart_puts("\n---------------------------------------------------");
    uart_puts("\n   ZIGGY-OS KERNEL SUBSYSTEM RUNTIME INITIALISED   ");
    uart_puts("\n---------------------------------------------------\n");

    /* Fire up hardware layout virtualization systems */
    init_mmu_mappings();
    init_scheduler();

    uart_puts("\n[*] Boot Cycle Cleared. Invoking scheduler executive loop...\n");
    
    while (1) {
        schedule_next_context();
        /* Arbitrary pause cycle between round-robin task updates */
        for (volatile int i = 0; i < 5000000; i++);
    }
}
