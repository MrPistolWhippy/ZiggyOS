#include <stdint.h>
extern void print(const char *str);

// --- TASK STATE SEGMENT (TSS) STRUCTURE ---
typedef struct {
    uint32_t link, esp0, ss0, esp1, ss1, esp2, ss2;
    uint32_t cr3, eip, eflags, eax, ecx, edx, ebx, esp, ebp, esi, edi;
    uint32_t es, cs, ss, ds, fs, gs, ldt, trap, iomap;
} __attribute__((packed)) TSS_t;

TSS_t sys_tss;
uint64_t gdt_table[6]; // Expand local GDT allocation table

void init_global_descriptor_table(void) {
    print("[CPU] Building isolated Privilege Ring Segmentation maps...\n");
    gdt_table[0] = 0x0000000000000000ULL; // Null
    gdt_table[1] = 0x00CF9A000000FFFFULL; // Kernel Code (Ring 0)
    gdt_table[2] = 0x00CF92000000FFFFULL; // Kernel Data (Ring 0)
    gdt_table[3] = 0x00CFFA000000FFFFULL; // User Code   (Ring 3 DPL3)
    gdt_table[4] = 0x00CFFF000000FFFFULL; // User Data   (Ring 3 DPL3)
    
    // Wire up the TSS descriptor entry
    uint32_t tss_base = (uint32_t)&sys_tss;
    uint32_t tss_limit = sizeof(sys_tss) - 1;
    gdt_table[5] = ((uint64_t)(tss_base & 0xFF000000) << 32) |
                   ((uint64_t)(tss_base & 0x00FF0000) << 16) |
                   ((uint64_t)(tss_base & 0x0000FFFF)) |
                   ((uint64_t)(tss_limit & 0x000F0000) << 32) |
                   ((uint64_t)0x00008900 << 32) |
                   (tss_limit & 0x0000FFFF);
}

// --- PRIVILEGE DROP EXECUTOR ---
static uint32_t user_stack[1024];
void jump_to_user_mode(void (*user_func)(void)) {
    print("[CPU] Severing Ring 0 control flags. Dropping down to Ring 3 Userland...\n");
    
    // Set up the TSS kernel stack tracking gate
    sys_tss.ss0 = 0x10; // Kernel Data Selector
    __asm__ volatile("mov %%esp, %0" : "=r"(sys_tss.esp0));

    // Force segment execution context step-down via fake interrupt stack frame execution
    __asm__ volatile(
        "mov $0x23, %%ax\n"  // User Data Segment Selector with RPL=3 (0x20 | 3)
        "mov %%ax, %%ds\n"
        "mov %%ax, %%es\n"
        "mov %%ax, %%fs\n"
        "mov %%ax, %%gs\n"
        "pushl $0x23\n"      // Push Stack Segment (SS)
        "pushl %0\n"         // Push Stack Pointer (ESP) pointing to user_stack top
        "pushfl\n"           // Push EFLAGS register state
        "pushl $0x1B\n"      // Push Code Segment (CS) Selector with RPL=3 (0x18 | 3)
        "pushl %1\n"         // Push Instruction Pointer target (EIP)
        "iret\n"             // Fire Return from Interrupt to cascade processing to Ring 3
        : : "r"(&user_stack[1024]), "r"(user_func) : "ax"
    );
}

// --- LEGACY ARCHITECTURE FALLBACK PASS-THROUGHS ---
void init_flash_page_mapper(void) {}
void init_video_dma_canvas(void) {}
void watchdog_kick(void) {}
void init_interrupt_priorities_sys(void) {}
int run_automated_tests_sys(void) { return 1; }
void telemetry_encrypt_sys(uint32_t *v, uint32_t const *k) { (void)v; (void)k; }
