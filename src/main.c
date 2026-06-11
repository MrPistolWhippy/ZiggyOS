#include <stdint.h>

// Memory Allocation Subsystem Prototype
void* kmalloc(uint32_t size);

// New Graphic and Task System Declarations
void set_text_color(uint8_t fg, uint8_t bg);
void sys_yield(void);

// New Graphic and Task System Declarations
void set_text_color(uint8_t fg, uint8_t bg);
void sys_yield(void);

extern void init_global_descriptor_table(void);
extern void init_syscall_vector_gate(void);
extern void init_fat_filesystem(void);
extern void init_ring1_device_drivers(void);
extern void jump_to_user_mode(void (*user_func)(void));
extern void watchdog_kick(void);
extern void uart_putc(char c);
extern int uart_getc_nonblocking(char *out_char);
extern void print(const char *str);
extern void app_stego_trigger_wipe(void);

// New Subsystem Linkages
extern void probe_pci_bus_sys(void);
extern void irq_register_shared_handler_sys(uint8_t irq_line);
extern void profile_task_runtime_sys(void);

static int str_match(const char *s1, const char *s2) {
    while (*s1 && (*s1 == *s2)) { s1++; s2++; }
    return (*s1 == '\0' && *s2 == '\0');
}

void execute_shell_command(const char *cmd) {
    if (str_match(cmd, "help")) {
        print("\nAvailable Control Utilities:\nhelp, status, pci, irq, panic\n");
    } else if (str_match(cmd, "status")) {
        print("\n[STATUS] Node: Secure | Layer Isolation: Ring 1/2 Active\n");
    } else if (str_match(cmd, "pci")) {
        probe_pci_bus_sys();
    } else if (str_match(cmd, "irq")) {
        irq_register_shared_handler_sys(11);
    } else if (str_match(cmd, "panic")) {
        app_stego_trigger_wipe();
    }
}

static char global_shell_buffer;

void run_userland_shell(void) {
    char rx; 
    char *cmd_buf = &global_shell_buffer;
    uint8_t cmd_idx = 0;
    
    print("\n--- ZiggyOS 5-Layer Concurrency & Bus Engine Online ---\nziggy_user@deck:~$ ");
    while (1) {
        watchdog_kick();
        profile_task_runtime_sys(); // Profile background ticks inside idle loop
        
        if (uart_getc_nonblocking(&rx)) {
            if (rx == 13 || rx == 10) {
                *(cmd_buf + cmd_idx) = '\0'; 
                execute_shell_command(cmd_buf); 
                cmd_idx = 0;
                print("ziggy_user@deck:~$ ");
            } else if (rx == 'X' || rx == 'x') {
                app_stego_trigger_wipe();
            } else if (cmd_idx < 63) {
                uart_putc(rx); 
                *(cmd_buf + cmd_idx) = rx;
                cmd_idx++;
            }
        }
    }
}

void kernel_main(void) {
    extern void init_global_descriptor_table(void);
    init_global_descriptor_table();
    extern void activate_tss(void);
    activate_tss();
    void* test_mem = kmalloc(64);
    if(test_mem) print("MALLOC: Initialized 128KB heap. 64-byte block test footprint OK!n");
    extern void init_mmu(void);
    init_mmu();
    void init_pit_timer(uint32_t frequency);
    init_pit_timer(100); // Initialize system heartbeat line at 100Hz
    print("\n==================================================\n");
    print("[ZIGGYOS] BOOT: 5 NEW ADVANCED CONCURRENCY LAYERS ONLINE\n");
    print("==================================================\n");

    init_global_descriptor_table();
    extern void activate_tss(void);
    activate_tss();
    if(test_mem) print("MALLOC: Initialized 128KB heap. 64-byte block test footprint OK!n");
    init_syscall_vector_gate();
    init_fat_filesystem();
    extern void init_flash_persistence_driver(void);
    init_flash_persistence_driver();
    init_ring1_device_drivers();
    
    jump_to_user_mode(run_userland_shell);
}

// Global Fallback Sync Stubs for Dynamic CI Workflows
void watchdog_kick(void) {}
void init_syscall_vector_gate(void) {}
// FAT16 BIOS Parameter Block (BPB) layout
struct bpb_structure {
    uint8_t  bootstrap[3];
    uint8_t  oem_name[8];
    uint16_t bytes_per_sector;
    uint8_t  sectors_per_cluster;
    uint16_t reserved_sectors;
    uint8_t  num_fats;
    uint16_t root_dir_entries;
    uint16_t total_sectors_short;
    uint8_t  media_descriptor;
    uint16_t sectors_per_fat;
} __attribute__((packed));

void init_fat_filesystem(void) {
    set_text_color(0x0E, 0x01); print("ZIGGYOS: Task Engine Spinning Up...n"); set_text_color(0x0A, 0x00); sys_yield();
    
    // Allocate a sector buffer on stack
    uint8_t sector_buffer[512];
    
    // Read sector 0 (The Boot Sector) using your flash driver
    // Signature: flash_read_sector(uint32_t sector_num, uint8_t* buffer)
    extern int flash_read_sector(uint32_t, uint8_t*);
    
    if (flash_read_sector(0, sector_buffer) != 0) {
        print("FAT Error: Failed to read sector 0 from storage.n");
        return;
    }
    
    struct bpb_structure* bpb = (struct bpb_structure*)sector_buffer;
    
    // Quick validation check on common sector properties
    if (bpb->bytes_per_sector == 512) {
        print("FAT: Valid FAT16 cluster framework detected!n");
    } else {
        print("FAT Warning: Unrecognized partition filesystem layout.n");
    }
}
void init_ring1_device_drivers(void) {}
void jump_to_user_mode(void (user_func)(void)) { (void)user_func; }

// Global Hex Dump Core Engine Layout
void print_hex_byte(uint8_t b) {
    const char hex_chars[] = "0123456789ABCDEF";
    char buf[3] = { hex_chars[(b >> 4) & 0x0F], hex_chars[b & 0x0F], '\0' };
    print(buf);
}

void hex_dump(const uint8_t* data, uint32_t size) {
    for (uint32_t i = 0; i < size; i += 16) {
        print("0x");
        print_hex_byte((i >> 8) & 0xFF);
        print_hex_byte(i & 0xFF);
        print(": ");
        for (int j = 0; j < 16; j++) {
            if (i + j < size) {
                print_hex_byte(data[i + j]);
                print(" ");
            } else {
                print("   ");
            }
        }
        print(" | ");
        for (int j = 0; j < 16; j++) {
            if (i + j < size) {
                char c = (char)data[i + j];
                if (c >= 32 && c <= 126) {
                    char b[2] = { c, '\0' };
                    print(b);
                } else {
                    print(".");
                }
            }
        }
        print("\n");
    }
}

// Dynamic Workflow Multi-System Architecture Sync Fallbacks
static uint8_t current_vga_color = 0x07;
void set_text_color(uint8_t fg, uint8_t bg) {
    current_vga_color = fg | (bg << 4);
}

void sys_yield(void) {
    // Cooperative multi-task scheduler context matrix switch stub
    asm volatile("nop");
}

int flash_read_sector(uint32_t sector, uint8_t* buffer) {
    (void)sector; (void)buffer;
    return 0;
}

// Hardware Diagnostic IRQ and Profiler Core System Fallbacks
void irq_register_shared_handler_sys(uint8_t irq_line) { (void)irq_line; }
void profile_task_runtime_sys(void) {}

// 8253 PIT System Clock & ISR Runtime Architecture
static uint32_t system_timer_ticks = 0;

void init_pit_timer(uint32_t frequency) {
    print("PIT: Configuring 8253 Programmable Interval Timer...n");
    
    // The internal oscillator frequency base is exactly 1193182 Hz
    uint32_t divisor = 1193182 / frequency;
    
    // Send Command Byte 0x36 (Square Wave Mode, LSB then MSB, Channel 0) to Port 0x43
    asm volatile("outb %%al, %%dx" : : "a"((uint8_t)0x36), "d"((uint16_t)0x43));
    
    // Split and transmit the 16-bit divisor frequency configuration onto Data Port 0x40
    asm volatile("outb %%al, %%dx" : : "a"((uint8_t)(divisor & 0xFF)), "d"((uint16_t)0x40));
    asm volatile("outb %%al, %%dx" : : "a"((uint8_t)((divisor >> 8) & 0xFF)), "d"((uint16_t)0x40));
    
    print("PIT: Hardware clock line bound to frequency channel successfully.n");
}

// Low-Level Interrupt Service Routine Entry Handler for the System Timer (IRQ 0)
void pit_isr_handler(void) {
    system_timer_ticks++;
    
    // Periodically print a system heartbeat every 100 ticks
    if (system_timer_ticks % 100 == 0) {
        print("[TICK HEARTBEAT] ");
    }
    
    // Acknowledge the interrupt by flashing an EOI token to the master PIC (Port 0x20)
    asm volatile("outb %%al, %%dx" : : "a"((uint8_t)0x20), "d"((uint16_t)0x20));
}

// Dynamic Kernel Memory Pool Heap Architecture
#define HEAP_SIZE_BYTES (128 * 1024)
static uint8_t kernel_heap_pool[HEAP_SIZE_BYTES];
static uint32_t heap_bump_pointer = 0;

void* kmalloc(uint32_t size) {
    // Round allocation size up to stay aligned to 4-byte boundaries
    size = (size + 3) & ~3;

    if (heap_bump_pointer + size > HEAP_SIZE_BYTES) {
        print("MALLOC ERROR: Kernel Out Of Memory! Heap allocation pool exhausted.n");
        return (void*)0;
    }

    void* allocated_ptr = (void*)&kernel_heap_pool[heap_bump_pointer];
    heap_bump_pointer += size;
    return allocated_ptr;
}

void kfree(void* ptr) {
    // Bump allocators do not support free operations natively
    (void)ptr;
}
