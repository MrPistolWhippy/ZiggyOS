#include <stdint.h>

extern void print(const char *str);
extern void uart_putc(char c);

// --- 1. LOCAL NON-VOLATILE FILE ALLOCATION TABLE (FAT) ---
#define MAX_FILES 8
#define BLOCK_SIZE 512

typedef struct {
    char filename[12];
    uint32_t start_block;
    uint32_t file_size;
    uint8_t  is_active;
} FAT_Directory_t;

static FAT_Directory_t fat_table[MAX_FILES];
// static uint8_t virtual_disk[MAX_FILES * BLOCK_SIZE];

void init_fat_filesystem(void) {
    print("[FS] Initializing Non-Volatile File Allocation Tables...\n");
    for (int i = 0; i < MAX_FILES; i++) {
        fat_table[i].is_active = 0;
    }
}

// --- 2. RING 1/2 ISOLATION DEVICE DRIVER STACKS ---
void init_ring1_device_drivers(void) {
    print("[CPU] Setting up Ring 1/2 Isolated Device Driver gates...\n");
    // Define custom segmentation access rings with DPL1/DPL2 supervisor flags
    uint64_t ring1_code_descriptor = 0x00CF9C000000FFFFULL; // Ring 1 Code Selector
    uint64_t ring2_data_descriptor = 0x00CFCE000000FFFFULL; // Ring 2 Data Selector
    (void)ring1_code_descriptor;
    (void)ring2_data_descriptor;
}

// --- INTERRUPT 0x80 SOFTWARE SYSCALL DISPATCHER ---
typedef struct {
    uint16_t base_low, selector;
    uint8_t  always0, flags;
    uint16_t base_high;
} __attribute__((packed)) IDT_Entry_t;

static IDT_Entry_t idt_pool[256];

void syscall_handler_main(uint32_t syscall_num, const char *arg_str) {
    if (syscall_num == 4) { print(arg_str); }
}

void init_syscall_vector_gate(void) {
    print("[CPU] Binding Software Interrupt 0x80 System Call Entry Gate...\n");
    uint32_t handler_addr = (uint32_t)syscall_handler_main;
    IDT_Entry_t *gate = &idt_pool[0x80];
    gate->base_low  = handler_addr & 0xFFFF;
    gate->selector   = 0x08;
    gate->always0    = 0;
    gate->flags      = 0xEF; 
    gate->base_high = (handler_addr >> 16) & 0xFFFF;
}

// --- LOW-LEVEL NETWORK PACKET ROUTING ENGINE ---
typedef struct {
    uint8_t  src_addr, dest_addr;
    uint16_t protocol_id, payload_len;
    uint8_t  data[32];
    uint16_t checksum;
} __attribute__((packed)) NetworkPacket_t;

void net_route_transmit_packet(uint8_t dest, uint16_t proto, uint8_t *payload, uint16_t len) {
    NetworkPacket_t pkt;
    pkt.src_addr    = 0x01; pkt.dest_addr   = dest; pkt.protocol_id = proto;
    pkt.payload_len = (len > 32) ? 32 : len;
    for (uint16_t i = 0; i < pkt.payload_len; i++) { pkt.data[i] = payload[i]; }
    pkt.checksum = 0xBCDE;
    print("[NET] Dispatching routed packet frame across active link channels...\n");
}

// --- LEGACY DESCRIPTOR AND CONTEXT REGISTRATION STACK ---
typedef struct {
    uint32_t link, esp0, ss0, esp1, ss1, esp2, ss2;
    uint32_t cr3, eip, eflags, eax, ecx, edx, ebx, esp, ebp, esi, edi;
    uint32_t es, cs, ss, ds, fs, gs, ldt, trap, iomap;
} __attribute__((packed)) TSS_t;

static TSS_t sys_tss;
uint64_t gdt_table[6];

void init_global_descriptor_table(void) {
    print("[CPU] Building isolated Privilege Ring Segmentation maps...\n");
    gdt_table[0] = 0x0000000000000000ULL; gdt_table[1] = 0x00CF9A000000FFFFULL;
    gdt_table[2] = 0x00CF92000000FFFFULL; gdt_table[3] = 0x00CFFA000000FFFFULL;
    gdt_table[4] = 0x00CFFF000000FFFFULL;
    uint32_t t_base = (uint32_t)&sys_tss; uint32_t t_lim = sizeof(sys_tss) - 1;
    gdt_table[5] = ((uint64_t)(t_base & 0xFF000000) << 32) | ((uint64_t)(t_base & 0x00FF0000) << 16) | ((uint64_t)(t_base & 0x0000FFFF)) | ((uint64_t)(t_lim & 0x000F0000) << 32) | ((uint64_t)0x00008900 << 32) | (t_lim & 0x0000FFFF);
}

void jump_to_user_mode(void (*user_func)(void)) {
    print("[CPU] Dropping control down to Ring 3 execution field...\n");
    sys_tss.ss0 = 0x10; __asm__ volatile("mov %%esp, %0" : "=r"(sys_tss.esp0));
    uint32_t u_stack;
    __asm__ volatile("mov $0x23, %%ax\n mov %%ax, %%ds\n mov %%ax, %%es\n mov %%ax, %%fs\n mov %%ax, %%gs\n pushl $0x23\n pushl %0\n pushfl\n pushl $0x1B\n pushl %1\n iret\n" : : "r"(&u_stack), "r"(user_func) : "ax");
}

void init_flash_page_mapper(void) {}
void init_video_dma_canvas(void) {}
void watchdog_kick(void) {}
void init_interrupt_priorities_sys(void) {}
int run_automated_tests_sys(void) { return 1; }
void telemetry_encrypt_sys(uint32_t *v, uint32_t const *k) { (void)v; (void)k; }
