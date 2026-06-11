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
gdt[num].base_low = (base & 0xFFFF);
gdt[num].base_middle = (base >> 16) & 0xFF;
gdt[num].base_high = (base >> 24) & 0xFF;
gdt[num].limit_low = (limit & 0xFFFF);
gdt[num].granularity = (limit >> 16) & 0x0F;
gdt[num].granularity |= gran & 0xF0;
gdt[num].access = access;
}

void init_gdt() {
gp.limit = (sizeof(struct gdt_entry) * 3) - 1;
gp.base = (unsigned int)&gdt;
gdt_set_gate(0, 0, 0, 0, 0);
gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);
gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);
__asm__ __volatile__("lgdt %0" : : "m"(gp));
}

unsigned int heap_start = 0x100000;
unsigned int heap_current = 0x100000;

void* malloc(unsigned int size) {
unsigned int alloc_address = heap_current;
heap_current += size;
if (heap_current >= 0x400000) {
return 0;
}
return (void*)alloc_address;
}

void free(void* ptr, unsigned int size) {
unsigned int addr = (unsigned int)ptr;
if (addr + size == heap_current) {
heap_current = addr;
}
}

unsigned short mmu_get_base_memory() {
volatile unsigned short* bios_ptr = (volatile unsigned short*)0x413;
return *bios_ptr;
}

struct idt_entry {
unsigned short base_lo;
unsigned short sel;
unsigned char always0;
unsigned char flags;
unsigned short base_hi;
} __attribute__((packed));

struct idt_ptr {
unsigned short limit;
unsigned int base;
} __attribute__((packed));

struct idt_entry idt[256];
struct idt_ptr idtp;

void idt_set_gate(unsigned char num, unsigned long base, unsigned short sel, unsigned char flags) {
idt[num].base_lo = (base & 0xFFFF);
idt[num].base_hi = (base >> 16) & 0xFFFF;
idt[num].sel = sel;
idt[num].always0 = 0;
idt[num].flags = flags;
}

void init_idt() {
idtp.limit = (sizeof(struct idt_entry) * 256) - 1;
idtp.base = (unsigned int)&idt;
for(int i = 0; i < 256; i++) idt_set_gate(i, 0, 0, 0);
__asm__ __volatile__("lidt %0" : : "m"(idtp));
}

void pic_remap() {
	outb(0x20, 0x11);
	outb(0xA0, 0x11);
	outb(0x21, 0x20);
	outb(0xA1, 0x28);
	outb(0x21, 0x04);
	outb(0xA1, 0x02);
	outb(0x21, 0x01);
	outb(0xA1, 0x01);
	outb(0x21, 0x0);
	outb(0xA1, 0x0);
}

void play_sound(unsigned int nFrequence) {
unsigned int Div;
unsigned char tmp;

// Calculate the 8253 timer divisor based on the requested pitch frequency
Div = 1193180 / nFrequence;

// Set command byte to prepare Channel 2 square wave generation
outb(0x43, 0xB6);
outb(0x42, (unsigned char) (Div & 0xFF));
outb(0x42, (unsigned char) ((Div >> 8) & 0xFF));

// Read Port 0x61, then turn on bits 0 and 1 to connect the speaker output
tmp = inb(0x61);
if (tmp != (tmp | 3)) {
outb(0x61, tmp | 3);
}
}

void nosound() {
unsigned char tmp = inb(0x61) & 0xFC; // Turn off bits 0 and 1 to mute
outb(0x61, tmp);
}

void mmu_trigger_beep() {
play_sound(440); // Standard 440Hz "Concert A" pitch tone
for(volatile int i = 0; i < 20000000; i++); // Short loop delay interval block
nosound();
}
