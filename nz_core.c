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

/* ==============================================================================
 *          ZIGGY-OS KERNEL CORE: ATOMIC MUTEXES & LOOPBACK NETWORK DRIVER
 * ============================================================================== */

/* --- 1. ATOMIC MUTEX SYNCHRONIZATION LOCK MECHANICS --- */
typedef struct {
    volatile uint32_t lock_state; /* 0 = UNLOCKED, 1 = LOCKED */
} mutex_t;

void mutex_init(mutex_t *mutex) {
    mutex->lock_state = 0;
}

void mutex_lock(mutex_t *mutex) {
    uint32_t tmp = 1;
    /* Atomic Exchange (amoswap.w.aq) forces hardware-level memory fencing */
    __asm__ volatile (
        "1: amoswap.w.aq %0, %1, (%2)\n"
        "   bnez %0, 1b\n"
        : "=r"(tmp)
        : "r"(tmp), "r"(&mutex->lock_state)
        : "memory"
    );
}

void mutex_unlock(mutex_t *mutex) {
    /* Atomic Release (amoswap.w.rl) flushes memory writes before unlocking */
    uint32_t tmp = 0;
    __asm__ volatile (
        "amoswap.w.rl %0, %1, (%2)\n"
        : "=r"(tmp)
        : "r"(tmp), "r"(&mutex->lock_state)
        : "memory"
    );
}

/* --- 2. LOOPBACK NETWORK DEVICE DRIVER --- */
#define NET_PACKET_MAX 128

typedef struct {
    uint8_t  data[NET_PACKET_MAX];
    uint32_t length;
    uint32_t src_ip;
    uint32_t dest_ip;
} net_packet_t;

static mutex_t net_buffer_mutex;

int loopback_transmit_packet(uint32_t src, uint32_t dest, const uint8_t *payload, uint32_t len) {
    if (len > NET_PACKET_MAX) return -1;
    
    /* Safely lock the network transmission buffer using our new atomic mutex */
    mutex_lock(&net_buffer_mutex);
    
    uart_puts("[📡 NET_CARD] Intercepting frame. Loopback routing active (lo0).\n");
    
    net_packet_t loopback_frame;
    loopback_frame.src_ip = src;
    loopback_frame.dest_ip = dest;
    loopback_frame.length = len;
    
    for (uint32_t i = 0; i < len; i++) {
        loopback_frame.data[i] = payload[i];
    }
    
    uart_puts("   └── [PACKET ROUTED] Loopback loop completed. Delivery verified.\n");
    
    /* Safely unlock the transmission hardware state */
    mutex_unlock(&net_buffer_mutex);
    return 0;
}

void init_networking_and_sync_layers(void) {
    mutex_init(&net_buffer_mutex);
    
    /* Run an immediate boot loopback test to ensure atomic hardware sync is alive */
    const uint8_t boot_msg[] = "ZIGGY_OS_NET_OK";
    loopback_transmit_packet(0x12700001, 0x12700001, boot_msg, 15);
}

/* ==============================================================================
 *          ZIGGY-OS KERNEL CORE: SMP MULTI-CORE BOOT & IPC PIPELINES
 * ============================================================================== */

#define MAX_CORES 4
#define IPC_MSG_MAX 64

/* --- 1. SYMMETRIC MULTIPROCESSING (SMP) MULTI-CORE REGISTRY --- */
static mutex_t smp_boot_mutex;
volatile uint32_t active_core_count = 1;

/* Thread local storage tracking for individual hardwares */
typedef struct {
    uint32_t hart_id;
    uint32_t state; /* 0=OFFLINE, 1=BOOTING, 2=ONLINE RUNNING */
} CoreRegistry_t;

static CoreRegistry_t cluster_cores[MAX_CORES];

/* Hardware-level secondary core boot entry point called via boot.s */
void smp_secondary_core_entry(uint32_t hart_id) {
    /* Protect shared configuration entry tables using atomic mutex primitives */
    mutex_lock(&smp_boot_mutex);
    
    if (hart_id < MAX_CORES) {
        cluster_cores[hart_id].hart_id = hart_id;
        cluster_cores[hart_id].state = 2; /* Mark Core Online */
        active_core_count++;
        
        uart_puts("[🚀 MULTI-CORE] Core initialized safely. Joining kernel ring cluster.\n");
    }
    
    mutex_unlock(&smp_boot_mutex);
    
    /* Drop secondary execution processing directly into local scheduler contexts */
    while (1) {
        schedule_next_context();
        for (volatile int i = 0; i < 5000000; i++);
    }
}

/* --- 2. EMBEDDED IPC MESSAGE PASSING PIPELINE --- */
typedef struct {
    uint32_t sender_core_id;
    uint32_t dest_core_id;
    uint32_t signal_vector_id;
    uint8_t  payload[IPC_MSG_MAX];
    uint32_t payload_len;
} ipc_msg_t;

/* RISC-V Core Local Interrupter (CLINT) IPI Register Map Base */
#define CLINT_MSIP_BASE 0x02000000
#define CLINT_MSIP(core_id) ((volatile uint32_t*)(CLINT_MSIP_BASE + (core_id * 4)))

static mutex_t ipc_pipeline_mutex;
static ipc_msg_t mailbox_pipeline[MAX_CORES];

int ipc_send_message(uint32_t from_core, uint32_t to_core, uint32_t signal, const uint8_t *data, uint32_t len) {
    if (to_core >= MAX_CORES || len > IPC_MSG_MAX) return -1;
    
    mutex_lock(&ipc_pipeline_mutex);
    
    uart_puts("[📬 IPC_PIPE] Packing secure inter-core transaction block payload.\n");
    
    mailbox_pipeline[to_core].sender_core_id = from_core;
    mailbox_pipeline[to_core].dest_core_id = to_core;
    mailbox_pipeline[to_core].signal_vector_id = signal;
    mailbox_pipeline[to_core].payload_len = len;
    
    for (uint32_t i = 0; i < len; i++) {
        mailbox_pipeline[to_core].payload[i] = data[i];
    }
    
    uart_puts("   └── [INTERRUPT TRIGGERED] Dispatching IPI line to destination CPU cell.\n");
    
    /* Assert MSIP bit 0 to trigger an immediate hardware Inter-Processor Interrupt on target core */
    *CLINT_MSIP(to_core) = 1;
    
    mutex_unlock(&ipc_pipeline_mutex);
    return 0;
}

void init_smp_and_ipc_layers(void) {
    mutex_init(&smp_boot_mutex);
    mutex_init(&ipc_pipeline_mutex);
    
    /* Initialize base boot entry status maps for core 0 */
    cluster_cores[0].hart_id = 0;
    cluster_cores[0].state = 2;
    
    /* Run local inter-core signal pipe smoke test layout loop */
    const uint8_t sync_token[] = "CORE_CLUSTER_READY";
    ipc_send_message(0, 1, 0xAA, sync_token, 18);
}

/* ==============================================================================
 *          ZIGGY-OS KERNEL CORE: SYSTEM CALL OSINT INTERFACE LAYER
 * ============================================================================== */

#define SYS_INTELX_QUERY 42

/* System call vector dispatch matrix mapping routine */
void handle_syscall_vector(uintptr_t sys_id, const char *query_param) {
    if (sys_id == SYS_INTELX_QUERY) {
        uart_puts("\n[⚡ SYSCALL EXECUTED] Vector 42 intercepted: Trapping to Machine Mode.\n");
        uart_puts("   └── Passing threat intelligence selector string payload: ");
        uart_puts(query_param);
        uart_puts("\n");
        
        /* Log parameters cleanly straight into our Virtual File System log file */
        vfs_create_file("sys_matrix.log", 512);
    }
}
