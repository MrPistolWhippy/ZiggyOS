#include "io.h"
struct gdt_entry {
	unsigned short limit_low;
	unsigned short base_low;
	unsigned char base_middle;
	unsigned char access;
	unsigned char granularity;
	unsigned char base_high;
} __attribute__((packed));
struct gdt_ptr {
	unsigned short limit;
	unsigned int base;
} __attribute__((packed));
struct gdt_entry gdt[3];
struct gdt_ptr gp;
void gdt_set_gate(int num, unsigned int base, unsigned int limit, unsigned char access, unsigned char gran) {
	gdt[num].base_low = (base \& 0xFFFF);
	gdt[num].base_middle = (base >> 16) \& 0xFF;
	gdt[num].base_high = (base >> 24) \& 0xFF;
	gdt[num].limit_low = (limit \& 0xFFFF);
	gdt[num].granularity = (limit >> 16) \& 0x0F;
	gdt[num].granularity |= gran \& 0xF0;
	gdt[num].access = access;
}
void init_gdt() {
	gp.limit = (sizeof(struct gdt_entry) * 3) - 1;
	gp.base = (unsigned int)\&gdt;
	gdt_set_gate(0, 0, 0, 0, 0);
	gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);
	gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);
	extern void gdt_flush(unsigned int);
	gdt_flush((unsigned int)\&gp);
}

unsigned short mmu_get_base_memory() {
	volatile unsigned short* bios_ptr = (volatile unsigned short*)0x413;
	return *bios_ptr;
}

unsigned int heap_start = 0x100000;
unsigned int heap_current = 0x100000;
void* malloc(unsigned int size) {
	unsigned int alloc_address = heap_current;
	heap_current += size;
	
	if (heap_current >= 0x400000) {
		print("Error: Kernel Heap Out of Memory!n");
		return 0;
	}
	return (void*)alloc_address;
}
