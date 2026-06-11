#include <stdint.h>

// Individual GDT segment gate attribute layout matrix
struct gdt_entry_bits {
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t  base_middle;
    uint8_t  access;
    uint8_t  granularity;
    uint8_t  base_high;
} __attribute__((packed));

struct gdt_ptr_bits {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

// Declare a standard 5-descriptor segment matrix array
static struct gdt_entry_bits gdt[5];
static struct gdt_ptr_bits   gdt_ptr;

extern void print(const char* str);

// Explicit segment gate encoder utility
static void set_gdt_gate(int num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran) {
    gdt[num].base_low    = (base & 0xFFFF);
    gdt[num].base_middle = (base >> 16) & 0xFF;
    gdt[num].base_high   = (base >> 24) & 0xFF;
    gdt[num].limit_low   = (limit & 0xFFFF);
    gdt[num].granularity = (limit >> 16) & 0x0F;
    gdt[num].granularity |= gran & 0xF0;
    gdt[num].access      = access;
}

void init_global_descriptor_table(void) {
    print("GDT: Constructing hardware segment descriptors layer...\n");

    // Configure memory structural boundaries (Limit: 4GB page granular flat memory model)
    gdt_ptr.limit = (sizeof(struct gdt_entry_bits) * 5) - 1;
    gdt_ptr.base  = (uint32_t)&gdt;

    // Descriptor Index 0: Null Descriptor requirement
    set_gdt_gate(0, 0, 0, 0, 0);
    // Index 1: Kernel Code Segment Descriptor Selector (Ring 0 Access: 0x9A)
    set_gdt_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);
    // Index 2: Kernel Data Segment Descriptor Selector (Ring 0 Access: 0x92)
    set_gdt_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);
    // Index 3: Userland Code Segment Descriptor Selector (Ring 3 Privilege Access: 0xFA)
    set_gdt_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF);
    // Index 4: Userland Data Segment Descriptor Selector (Ring 3 Privilege Access: 0xF2)
    set_gdt_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF);

    // Assembly context binding sequence: flush GDT registry address and reload tracking registers
    asm volatile(
        "lgdt %0\n\t"
        "mov $0x10, %%ax\n\t"
        "mov %%ax, %%ds\n\t"
        "mov %%ax, %%es\n\t"
        "mov %%ax, %%fs\n\t"
        "mov %%ax, %%gs\n\t"
        "mov %%ax, %%ss\n\t"
        "ljmp $0x08, $.flush_gdt\n\t"
        ".flush_gdt:\n\t"
        :
        : "m"(gdt_ptr)
        : "ax", "memory"
    );

    print("GDT: Memory tracking segment registry successfully loaded into CPU registers!\n");
}
