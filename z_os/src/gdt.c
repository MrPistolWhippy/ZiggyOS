#include <stdint.h>

// Standard x86 GDT Entry entry layout structure
struct gdt_entry_struct {
    uint16_t limit_low;           // The lower 16 bits of the memory limit
    uint16_t base_low;            // The lower 16 bits of the base address
    uint8_t  base_middle;         // The next 8 bits of the base address
    uint8_t  access;              // Access flags (determines privilege Ring 0 vs Ring 3)
    uint8_t  granularity;         // Size modifiers and high limit bits
    uint8_t  base_high;           // The last 8 bits of the base address
} __attribute__((packed));

struct gdt_ptr_struct {
    uint16_t limit;               // Total size of the GDT minus 1
    uint32_t base;                // The memory address where the table starts
} __attribute__((packed));

// We need 3 entries: 1 Null descriptor, 1 Flat Code segment, and 1 Flat Data segment
struct gdt_entry_struct gdt_entries[3];
struct gdt_ptr_struct   gdt_reg;

// External assembly routine to load the new segments into the CPU registers
extern void gdt_flush(uint32_t gdt_ptr_address);

void set_gdt_gate(int32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran) {
    gdt_entries[num].base_low    = (base & 0xFFFF);
    gdt_entries[num].base_middle = (base >> 16) & 0xFF;
    gdt_entries[num].base_high   = (base >> 24) & 0xFF;

    gdt_entries[num].limit_low   = (limit & 0xFFFF);
    gdt_entries[num].granularity = (limit >> 16) & 0x0F;

    gdt_entries[num].granularity |= gran & 0xF0;
    gdt_entries[num].access      = access;
}

void init_gdt(void) {
    gdt_reg.limit = (sizeof(struct gdt_entry_struct) * 3) - 1;
    gdt_reg.base  = (uint32_t)&gdt_entries;

    // 1. Entry 0: The compulsory x86 Null Descriptor
    set_gdt_gate(0, 0, 0, 0, 0);

    // 2. Entry 1: Flat Code Segment descriptor (Base 0, Limit 4GB, Ring 0 Execution)
    // Access: 0x9A (Kernel Code), Granularity: 0xCF (4KB blocks, 32-bit mode)
    set_gdt_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);

    // 3. Entry 2: Flat Data Segment descriptor (Base 0, Limit 4GB, Ring 0 Read/Write)
    // Access: 0x92 (Kernel Data), Granularity: 0xCF
    set_gdt_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);

    // Load the GDT register and perform the far jump to reload segment selectors
    gdt_flush((uint32_t)&gdt_reg);
}
