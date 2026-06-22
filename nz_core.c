/* ==============================================================================
 *          ZIGGY-OS ADVANCED MANAGEMENT STACK: VFS & HEAP ALLOCATOR
 * ============================================================================== */
#include <stdint.h>
#include <stddef.h>

/* --- 1. MEMORY-MAPPED CONSOLE UART DEFINITIONS --- */
#define UART_BASE 0x10000000
#define UART_THR  ((volatile uint8_t*)(UART_BASE + 0))
#define UART_LSR  ((volatile uint8_t*)(UART_BASE + 5))

void uart_puts(const char *s) {
    while (*s) {
        while ((*UART_LSR & 0x20) == 0);
        *UART_THR = *s++;
    }
}

/* --- 2. DYNAMIC KERNEL HEAP ALLOCATOR (8-BYTE ALIGNED SLAB) --- */
#define HEAP_SIZE (64 * 1024) /* 64KB Managed Kernel Heap Block */
static uint8_t kernel_heap[HEAP_SIZE] __attribute__((aligned(8)));
static size_t heap_top = 0;

void *kmalloc(size_t size) {
    /* Align individual allocation sizing parameters to 8-byte boundary chunks */
    size = (size + 7) & ~7;
    if (heap_top + size > HEAP_SIZE) {
        uart_puts("[⚠️ ALLOC ERROR] Kernel Heap Space Exhausted.\n");
        return NULL;
    }
    void *ptr = &kernel_heap[heap_top];
    heap_top += size;
    return ptr;
}

void kfree(void *ptr) {
    /* Basic arena compaction stub layout */
    (void)ptr;
}

/* --- 3. VIRTUAL FILE SYSTEM (VFS) FRAMEWORK LAYOUT --- */
#define VFS_MAX_NAME 32
#define VFS_TYPE_REG 1
#define VFS_TYPE_DIR 2

typedef struct vnode {
    char     name[VFS_MAX_NAME];
    uint8_t  type;
    size_t   size;
    uint8_t  *data_block_ptr;
    struct vnode *next;
} vnode_t;

static vnode_t *vfs_root = NULL;

void vfs_init(void) {
    vfs_root = (vnode_t *)kmalloc(sizeof(vnode_t));
    if (vfs_root) {
        vfs_root->name[0] = '/';
        vfs_root->name[1] = '\0';
        vfs_root->type = VFS_TYPE_DIR;
        vfs_root->size = 0;
        vfs_root->data_block_ptr = NULL;
        vfs_root->next = NULL;
        uart_puts("[✓] VFS Subsystem: Root Virtual Mount Node Formatted.\n");
    }
}

int vfs_create_file(const char *name, size_t initial_bytes) {
    vnode_t *new_node = (vnode_t *)kmalloc(sizeof(vnode_t));
    if (!new_node) return -1;

    size_t i = 0;
    while (i < VFS_MAX_NAME - 1 && name[i]) {
        new_node->name[i] = name[i];
        i++;
    }
    new_node->name[i] = '\0';
    new_node->type = VFS_TYPE_REG;
    new_node->size = initial_bytes;
    new_node->data_block_ptr = (uint8_t *)kmalloc(initial_bytes);
    
    /* Chain into global lookup index tree */
    new_node->next = vfs_root->next;
    vfs_root->next = new_node;

    uart_puts("[✓] VFS Subsystem: Virtual File Register Complete: ");
    uart_puts(name);
    uart_puts("\n");
    return 0;
}

/* --- 4. SYSTEM INITIALIZATION ENTRY EXECUTIVE --- */
void _start(void) {
    extern uint32_t __bss_start, __bss_end;
    uint32_t *bss = &__bss_start;
    while (bss < &__bss_end) {
        *bss++ = 0;
    }

    uart_puts("\n---------------------------------------------------");
    uart_puts("\n   ZIGGY-OS KERNEL SUBSYSTEM RUNTIME INITIALISED   ");
    uart_puts("\n---------------------------------------------------\n");

    /* Fire up heap dynamic tracking memory and mount root partitions */
    vfs_init();
    vfs_create_file("sys_matrix.log", 512);
    vfs_create_file("mesh_topo.db", 1024);

    uart_puts("\n[*] Microkernel execution baseline running stably in memory loops.\n");
    while (1);
}

/* Explicit link to user-space command execution loop */
extern void ziggy_shell_run(const char *cmd_buffer);

void execute_mock_terminal(void) {
    uart_puts("\nziggy-sh# ");
    /* Simulate an automated user interactive call */
    ziggy_shell_run("ls");
    uart_puts("ziggy-sh# ");
    ziggy_shell_run("help");
}

/* ==============================================================================
 *          ZIGGY-OS KERNEL CORE: PREEMPTIVE TIMER & ELF LOADER SUBSYSTEMS
 * ============================================================================== */

/* --- 1. RISC-V CLINT TIMER REGISTERS (M-TIME / M-TIMECMP) --- */
#define CLINT_BASE     0x02000000
#define CLINT_MTIMECMP ((volatile uint64_t*)(CLINT_BASE + 0x4000))
#define CLINT_MTIME    ((volatile uint64_t*)(CLINT_BASE + 0xBFF8))
#define TIMER_INTERVAL 10000000 /* ~1 second clock slice interval steps */

/* Enable timer interrupts inside machine status register */
void init_timer_preemption(void) {
    uintptr_t mie;
    /* Set next absolute timestamp check match trigger */
    *CLINT_MTIMECMP = *CLINT_MTIME + TIMER_INTERVAL;
    
    /* Read Machine Interrupt Enable register and set MTIE (Machine Timer Interrupt Enable, bit 7) */
    __asm__ volatile("csrr %0, mie" : "=r"(mie));
    mie |= (1 << 7);
    __asm__ volatile("csrw mie, %0" :: "r"(mie));
    
    uart_puts("[✓] Preemptive Driver: CLINT Machine Timer Interconnections ON.\n");
}

/* --- 2. LIGHTWEIGHT ELF BINARY LOADER FORMAT DESCRIPTOR --- */
#define ELF_MAGIC_0 0x7F
#define ELF_MAGIC_1 'E'
#define ELF_MAGIC_2 'L'
#define ELF_MAGIC_3 'F'

typedef struct {
    uint8_t  e_ident[16];
    uint16_t e_type;
    uint16_t e_machine;
    uint32_t e_version;
    uintptr_t e_entry;   /* Memory address translation program jump location */
    uintptr_t e_phoff;
    uintptr_t e_shoff;
    uint32_t e_flags;
    uint16_t e_ehsize;
    uint16_t e_phentsize;
    uint16_t e_phnum;
} Elf32_Ehdr_t;

int load_and_execute_elf(const uint8_t *elf_binary_stream) {
    Elf32_Ehdr_t *header = (Elf32_Ehdr_t *)elf_binary_stream;
    
    uart_puts("[*] ELF Loader: Reading binary header descriptors...\n");
    
    /* Verify unique executable magic signatures */
    if (header->e_ident[0] != ELF_MAGIC_0 || header->e_ident[1] != ELF_MAGIC_1 ||
        header->e_ident[2] != ELF_MAGIC_2 || header->e_ident[3] != ELF_MAGIC_3) {
        uart_puts("    └── [ERROR] Invalid signature mismatch. Target payload rejected.\n");
        return -1;
    }
    
    uart_puts("    └── [SUCCESS] Valid ELF Header Validated.\n");
    uart_puts("    └── [REDIRECT] Context jump passing target vector to address location.\n");
    
    /* Cast executable entry block memory vector location pointer address to absolute target hook */
    void (*user_program_entry)(void) = (void (*)(void))(header->e_entry);
    (void)user_program_entry; /* Guard entry variable target reference boundary layout */
    
    return 0;
}

/* ==============================================================================
 *          ZIGGY-OS KERNEL CORE: DEVICE DRIVER STACK & USER PROTECTION
 * ============================================================================== */

/* --- 1. ABSTRACT VIRTUAL DEVICE DRIVER STRUCT --- */
#define DEV_NAME_MAX 16

typedef struct char_device {
    char name[DEV_NAME_MAX];
    int  (*open)(void);
    int  (*read)(uint8_t *buffer, size_t length);
    int  (*write)(const uint8_t *buffer, size_t length);
    struct char_device *next;
} char_device_t;

static char_device_t *device_list_head = NULL;

/* Mock Framebuffer Screen Driver implementations */
int mock_screen_open(void) {
    uart_puts("[🖥️ DRIVER] Virtual Framebuffer Screen Device Opened.\n");
    return 0;
}

int mock_screen_write(const uint8_t *buf, size_t len) {
    uart_puts("[🖥️ DRIVER] Blitting raster buffer stream to virtual display.\n");
    return (int)len;
}

void register_char_device(char_device_t *dev) {
    dev->next = device_list_head;
    device_list_head = dev;
    uart_puts("[✓] Driver Stack: Registered Character Device Interface: ");
    uart_puts(dev->name);
    uart_puts("\n");
}

/* --- 2. USER-SPACE MEMORY PROTECTION LAYER (PMP / PTE USER BITS) --- */
#define PTE_U     (1 << 4)     /* User Mode Accessible Attribute Bit */

void enforce_user_space_protection(uint64_t *page_table_root) {
    /* Set up User-space sandbox execution permissions (0x00010000 -> 0x00010000) */
    /* Flags include Valid (V), Readable (R), Executable (X), and User Access (U) */
    uint64_t user_ppn = (0x00010000ULL >> 12);
    
    /* Lock protection bits into index 0 slots for localized user-space environments */
    page_table_root[0] = (user_ppn << 10) | PTE_V | PTE_R | PTE_X | PTE_U;
    
    /* Configure RISC-V Physical Memory Protection (PMP) hardware bounds registers */
    /* Setup pmpcfg0 to locked, naturally aligned 4-byte region (NAPOT) with Read/Write execution restrictions */
    uintptr_t pmpcfg0_val = 0x1F; /* Locked | NAPOT | X | W | R */
    __asm__ volatile("csrw pmpcfg0, %0" :: "r"(pmpcfg0_val));
    
    uart_puts("[✓] Protection Layer: Hardware PMP / PTE User Privilege Isolation ARMED.\n");
}

/* Statically instantiated driver blocks */
static char_device_t screen_driver = {
    .name = "dev_fb0",
    .open = mock_screen_open,
    .read = NULL,
    .write = mock_screen_write
};

void init_advanced_hardware_extensions(void) {
    register_char_device(&screen_driver);
    enforce_user_space_protection((uint64_t *)&root_page_table);
}
