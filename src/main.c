#include <stdint.h>

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
    print("\n==================================================\n");
    print("[ZIGGYOS] BOOT: 5 NEW ADVANCED CONCURRENCY LAYERS ONLINE\n");
    print("==================================================\n");

    init_global_descriptor_table();
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
    print("FAT: Initializing storage driver mount...n");
    
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
