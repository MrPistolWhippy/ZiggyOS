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

// Task State Segment Structure Definition
struct tss_entry_struct {
    uint32_t prev_tss;
    uint32_t esp0;       // Kernel stack pointer used on privilege ring switch
    uint32_t ss0;        // Kernel stack segment selector
    uint32_t esp1; uint32_t ss1; uint32_t esp2; uint32_t ss2;
    uint32_t cr3; uint32_t eip; uint32_t eflags;
    uint32_t eax; uint32_t ecx; uint32_t edx; uint32_t ebx;
    uint32_t esp; uint32_t ebp; uint32_t esi; uint32_t edi;
    uint32_t es; uint32_t cs; uint32_t ss; uint32_t ds; uint32_t fs; uint32_t gs;
    uint32_t ldt; uint16_t trap; uint16_t iomap_base;
} __attribute__((packed));

// Re-declare tracking variables to expand our array size to 6 descriptors
static struct gdt_entry_bits expanded_gdt[6];
static struct gdt_ptr_bits   expanded_gdt_ptr;
static struct tss_entry_struct tss_entry;

void write_tss(int32_t num, uint16_t ss0, uint32_t esp0) {
    uint32_t base = (uint32_t)&tss_entry;
    uint32_t limit = base + sizeof(tss_entry);

    // Initialize TSS fields to 0, then bind privilege tracking data
    for (uint32_t i = 0; i < sizeof(tss_entry); i++) ((uint8_t*)&tss_entry)[i] = 0;
    tss_entry.ss0 = ss0;
    tss_entry.esp0 = esp0;
    tss_entry.iomap_base = sizeof(tss_entry);

    // Add entry 5 to GDT: Access 0x89 (Present, Executable, Descriptor privilege ring 0, TSS type)
    expanded_gdt[num].base_low    = (base & 0xFFFF);
    expanded_gdt[num].base_middle = (base >> 16) & 0xFF;
    expanded_gdt[num].base_high   = (base >> 24) & 0xFF;
    expanded_gdt[num].limit_low   = (limit & 0xFFFF);
    expanded_gdt[num].granularity = ((limit >> 16) & 0x0F) | 0x00;
    expanded_gdt[num].access      = 0x89;
}

void activate_tss(void) {
    print("TSS: Registering Task State Segment and privilege hardware stack pointers...\n");
    
    // Duplicate existing entries into expanded array wrapper
    for(int i=0; i<5; i++) expanded_gdt[i] = gdt[i];
    expanded_gdt_ptr.limit = (sizeof(struct gdt_entry_bits) * 6) - 1;
    expanded_gdt_ptr.base  = (uint32_t)&expanded_gdt;

    // Write TSS descriptor into entry slot index 5 (Kernel Data Selector = 0x10)
    uint32_t kernel_stack_top;
    asm volatile("mov %%esp, %0" : "=r"(kernel_stack_top));
    write_tss(5, 0x10, kernel_stack_top);

    // Flush expanded GDT and explicitly load Task Register (0x28 = Index 5)
    asm volatile(
        "lgdt %0\n\t"
        "mov $0x28, %%ax\n\t"
        "ltr %%ax\n\t"
        :
        : "m"(expanded_gdt_ptr)
        : "ax", "memory"
    );
    print("TSS: Hardware stack ring bounds armed successfully!\n");
}
