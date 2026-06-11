#include "io.h"

unsigned int heap_start = 0x100000;
unsigned int heap_current = 0x100000;

void* malloc(unsigned int size) {
    unsigned int alloc_address = heap_current;
    heap_current += size;
    return (void*)alloc_address;
}

void free(void* ptr, unsigned int size) {
    (void)ptr; (void)size;
}

void mmu_hex_dump(void* start_addr) {
    unsigned char* ptr = (unsigned char*)start_addr;
    print("Memory Hex Dump:\n");
    for (int line = 0; line < 2; line++) {
        for (int byte = 0; byte < 16; byte++) {
            print_char(' '); ptr++;
        }
        print("\n");
    }
}

void play_sound(unsigned int nFrequence) {
    unsigned int Div = 1193180 / nFrequence;
    outb(0x43, 0xB6);
    outb(0x42, (unsigned char) (Div & 0xFF));
    outb(0x42, (unsigned char) ((Div >> 8) & 0xFF));
    unsigned char tmp = inb(0x61);
    if (tmp != (tmp | 3)) outb(0x61, tmp | 3);
}

void nosound() {
    outb(0x61, inb(0x61) & 0xFC);
}

void mmu_trigger_beep() {
    play_sound(440);
    for(volatile int i = 0; i < 5000000; i++);
    nosound();
}

void mmu_get_rtc_time(unsigned char* h, unsigned char* m, unsigned char* s) {
    *h = 12; *m = 0; *s = 0;
}

void mmu_get_rtc_date(unsigned char* d, unsigned char* mo, unsigned char* y) {
    *d = 1; *mo = 1; *y = 26;
}

void init_ramfs() {}
void init_gdt() {}
void init_idt() {}
void pic_remap() {}

// Global GDT fallback tracking handle
struct gdt_entry_stub {
    unsigned short limit_low; unsigned short base_low; unsigned char base_middle;
    unsigned char access; unsigned char granularity; unsigned char base_high;
};
struct gdt_entry_stub gdt;

void init_mouse() {}
void mouse_poll_movement() {}
