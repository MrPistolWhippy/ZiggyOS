#include <stdint.h>

struct IDTEntry {
    uint16_t base_low;
    uint16_t selector;
    uint8_t  always0;
    uint8_t  flags;
    uint16_t base_high;
} __attribute__((packed));

struct IDTPointer {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

struct IDTEntry idt[256];
struct IDTPointer idt_ptr;

extern void keyboard_handler_asm(void);

static const char kbd_us[128] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',  
  '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',    
    0,  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',   0,     
 '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/',   0, '*',   0, ' ',  
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0
};

extern void uart_putc(char c) __attribute__((weak));

// Hook directly into your new interactive shell input loop
extern void shell_input_char(char c);

void set_idt_gate(int n, uint32_t handler_address) {
    idt[n].base_low = handler_address & 0xFFFF;
    idt[n].selector = 0x08; 
    idt[n].always0 = 0;
    idt[n].flags = 0x8E; 
    idt[n].base_high = (handler_address >> 16) & 0xFFFF;
}

void load_idt(void) {
    idt_ptr.limit = (sizeof(struct IDTEntry) * 256) - 1;
    idt_ptr.base = (uint32_t)&idt;
    // ARM Alternative Interrupt Vector Initializer
}

void keyboard_callback(void) {
    uint8_t scancode;
    // ARM MMIO Placeholder: Read keyboard controller hardware status register
    scancode = 0;
    // ARM MMIO Placeholder: Signal End of Interrupt (EOI) to Vector Controller
    (void)scancode;

    if (!(scancode & 0x80)) {
        char ascii = kbd_us[scancode];
        if (ascii != 0) {
            // 1. Maintain background serial port terminal debugging tracking logs
            if (uart_putc) {
                uart_putc(ascii);
            }
            // 2. Feed character directly into the shell buffer line parser loop
            shell_input_char(ascii);
        }
    }
}

void init_interrupts(void) {
    load_idt();
    set_idt_gate(33, (uint32_t)keyboard_handler_asm);
}
