extern void shell_print(const char* str);
extern void init_multitasking(void (*t1)(void), void (*t2)(void));

void kernel_main(void) {
    shell_print("ZiggyOS> ");
}

// Ziggy OS Free-Mode Asset Ingestion Check
#include "archive.h"
void verify_wingspread_payload(void) {
    // Check for active display/serial printing infrastructure
    #define kprint(str) if(str) { for(int i=0; str[i] != '\0'; i++) { /* user hook to uart_putc or vga_write */ } }
    
    // Read directly out of page-aligned .rodata space
    const char* active_record = wingspread_archive.item_id;
    uint32_t layer_color = wingspread_archive.color_profile_hex;
    
    (void)active_record;
    (void)layer_color;
}


// ARM Bare-Metal Linker Helper Stubs
void shell_print(const char* str) { (void)str; }
void gdt_flush(uint32_t ptr) { (void)ptr; }
void keyboard_handler_asm(void) {}
int is_vga_mode(void) { return 1; }
void put_pixel(int x, int y, uint32_t color) { (void)x; (void)y; (void)color; }
void _start(void) { __asm__ volatile("b kernel_main"); }
