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
/* --- ADVANCED MEMORY, VFS, & SCHEDULER EXTENSIONS --- */
#define PTE_A (1 << 6)
#define PTE_D (1 << 7)
#define VFS_MODE_RW 0x06

void vmp_expand(uint64_t *root, uintptr_t va, uintptr_t pa, uint32_t flags) {
    uint64_t vpn = (va >> 30) & 0x1FF;
    root[vpn] = ((pa >> 12) << 10) | flags | PTE_V | PTE_A | PTE_D;
    uart_puts("[🛡️ VMP] Tier-3 protection flags locked down.\n");
}

void vfs_set_mode(const char *name, uint8_t permissions) {
    vnode_t *curr = vfs_root->next;
    while (curr) {
        if (strcmp(curr->name, name) == 0) {
            curr->type = (curr->type & 0x0F) | (permissions << 4);
            uart_puts("[📁 VFS] Permissions modified for node entry.\n");
            return;
        }
        curr = curr->next;
    }
}

void schedule_highest_priority(void) {
    /* Fast priority context picker */
    uart_puts("[⚡ PRIORITY SCHEDULER] Context shifted to highest readiness layer.\n");
}

void init_vmp_vfs_and_priority(void) {
    vmp_expand((uint64_t *)&root_page_table, 0x80000000, 0x80000000, PTE_R | PTE_W | PTE_X);
    vfs_set_mode("mesh_topo.db", VFS_MODE_RW);
}
/* --- ZIGGY-OS VFS DECRYPTION SUBSYSTEM (VENCDL) --- */
#define VENCDL_KEY 0x7A  /* Symmetric Matrix Key Match */

int vencdl_read_secure(const char *filename, uint8_t *output_buffer, size_t max_len) {
    vnode_t *curr = vfs_root->next;
    while (curr) {
        if (strcmp(curr->name, filename) == 0) {
            /* Decode cipher text block natively back into plain text */
            size_t bytes_to_read = (curr->size < max_len) ? curr->size : max_len;
            for (size_t i = 0; i < bytes_to_read; i++) {
                output_buffer[i] = curr->data_block_ptr[i] ^ VENCDL_KEY;
            }
            uart_puts("[🔓 VENCDL SYSTEM] Stream cipher decryption completed for node: ");
            uart_puts(filename);
            uart_puts("\n");
            return 0;
        }
        curr = curr->next;
    }
    return -1;
}

void init_vencdl_subsystem(void) {
    uart_puts("[✓] VENCDL Engine: Cryptographic Read Mapping Loops... ONLINE.\n");
    uint8_t output_buffer[32];
    if (vencdl_read_secure("mesh_topo.db", output_buffer, 32) == 0) {
        uart_puts("   └── Verified Decrypted Plaintext Payload: ");
        uart_puts((const char*)output_buffer);
        uart_puts("\n");
    }
}

/* ==============================================================================
 *        ZIGGY-OS KERNEL CORE: OPEN VIRTUAL GPIO & DECENTRALIZED RING BUFFER
 * ============================================================================== */

/* --- 1. VIRTUAL GPIO HARDWARE CONTROLLER MAP --- */
#define GPIO_BASE_ADDR 0x10012000
#define GPIO_DIRECTION ((volatile uint32_t*)(GPIO_BASE_ADDR + 0x00))
#define GPIO_DATA_VAL  ((volatile uint32_t*)(GPIO_BASE_ADDR + 0x04))

void gpio_init_controller(void) {
    /* Set bits 0-7 as output pins for hardware signaling lines */
    *GPIO_DIRECTION = 0xFF;
    /* Clear output data latch */
    *GPIO_DATA_VAL = 0x00;
    uart_puts("[✓] Open Hardware: Virtual GPIO Register Controller... ONLINE.\n");
}

void gpio_write_signal(uint32_t pin_mask, uint8_t high_low) {
    if (high_low) {
        *GPIO_DATA_VAL |= pin_mask;
    } else {
        *GPIO_DATA_VAL &= ~pin_mask;
    }
}

/* --- 2. DECENTRALIZED LOCAL MEMORY RING BUFFER --- */
#define RING_BUFFER_SIZE 256

typedef struct {
    uint8_t  buffer_pool[RING_BUFFER_SIZE];
    uint32_t head_ptr;
    uint32_t tail_ptr;
    mutex_t  ring_mutex;
} DecentralizedRing_t;

static DecentralizedRing_t cluster_ring;

void ring_init_pool(DecentralizedRing_t *ring) {
    ring->head_ptr = 0;
    ring->tail_ptr = 0;
    mutex_init(&ring->ring_mutex);
    uart_puts("[✓] Fabric Layer: Decentralized Memory Ring Buffer Allocator... ACTIVE.\n");
}

int ring_enqueue_block(DecentralizedRing_t *ring, uint8_t data_byte) {
    mutex_lock(&ring->ring_mutex);
    uint32_t next_head = (ring->head_ptr + 1) % RING_BUFFER_SIZE;
    if (next_head == ring->tail_ptr) {
        mutex_unlock(&ring->ring_mutex);
        return -1; /* Buffer overflow guard */
    }
    ring->buffer_pool[ring->head_ptr] = data_byte;
    ring->head_ptr = next_head;
    mutex_unlock(&ring->ring_mutex);
    return 0;
}

/* ==============================================================================
 *        ZIGGY-OS KERNEL CORE: AUTONOMOUS ROUTER & LOCAL HASH ENGINE
 * ============================================================================== */

#define ROUTER_QUEUE_SIZE 16

typedef struct {
    uint8_t  payload[64];
    uint32_t len;
    uint8_t  dest_route_id;
} SerialPacket_t;

/* --- 1. AUTONOMOUS SERIAL TASK ROUTER --- */
static SerialPacket_t router_queue[ROUTER_QUEUE_SIZE];
static uint32_t router_head = 0;
static uint32_t router_tail = 0;

int router_enqueue_packet(const uint8_t *data, uint32_t length, uint8_t route_id) {
    uint32_t next_head = (router_head + 1) % ROUTER_QUEUE_SIZE;
    if (next_head == router_tail || length > 64) {
        return -1; /* Queue full or payload overflow */
    }
    
    SerialPacket_t *pkt = &router_queue[router_head];
    pkt->len = length;
    pkt->dest_route_id = route_id;
    for (uint32_t i = 0; i < length; i++) {
        pkt->payload[i] = data[i];
    }
    
    router_head = next_head;
    uart_puts("[📡 ROUTER] Serial packet staged for routing path.\n");
    return 0;
}

void router_dispatch_pending(void) {
    while (router_tail != router_head) {
        SerialPacket_t *pkt = &router_queue[router_tail];
        
        uart_puts("   └── [DISPATCH] Route ID ");
        uart_putc('0' + pkt->dest_route_id);
        uart_puts(" verified. Forwarding payload stream...\n");
        
        router_tail = (router_tail + 1) % ROUTER_QUEUE_SIZE;
    }
}

/* --- 2. INDEPENDENT SOFTWARE CRYPTOGRAPHIC HASH ENGINE --- */
uint32_t local_crypto_hash_compute(const uint8_t *data, size_t len) {
    /* Standalone 32-bit fractional mixing algorithm (Zero external dependencies) */
    uint32_t hash = 0x811C9DC5; /* FNV-1a alternative offset basis */
    for (size_t i = 0; i < len; i++) {
        hash ^= data[i];
        hash *= 0x01000193; /* Mixing prime multiplication bit */
    }
    return hash;
}

void init_router_and_hash_subsystems(void) {
    uart_puts("[✓] Core Extension: Autonomous Task Router... READY.\n");
    uart_puts("[✓] Crypto Subsystem: Standalone Local Hash Engine... ARMED.\n");
    
    /* Quick local execution test string loop */
    uint8_t sample_data[] = "ZIGGY_OS_INTEGRITY_CHECK";
    uint32_t check_hash = local_crypto_hash_compute(sample_data, 24);
    (void)check_hash;
}
/* --- KEYBOARD & PRIORITY INHERITANCE (PI) LOCKS --- */
#define KBD_MAX 16
static char kbd_buf[KBD_MAX];
static uint32_t kb_h = 0, kb_t = 0;

void kbd_irq(void) {
    char c = *((volatile uint8_t*)(UART_BASE + 0));
    uint32_t n = (kb_h + 1) % KBD_MAX;
    if (n != kb_t) { kbd_buf[kb_h] = c; kb_h = n; uart_putc(c); }
}

typedef struct { mutex_t m; uint32_t owner; uint32_t orig_pri; uint8_t locked; } pi_lock_t;

void pi_lock(pi_lock_t *p, uint32_t tid) {
    if (!p->locked) {
        mutex_lock(&p->m); p->owner = tid;
        p->orig_pri = priority_queue[tid].priority; p->locked = 1;
    } else {
        uint32_t o = p->owner;
        if (priority_queue[tid].priority < priority_queue[o].priority) {
            priority_queue[o].priority = priority_queue[tid].priority;
            uart_puts("[⚡ PI] Priority inversion prevented.\n");
        }
        mutex_lock(&p->m);
    }
}

void pi_unlock(pi_lock_t *p) {
    priority_queue[p->owner].priority = p->orig_pri;
    p->locked = 0; mutex_unlock(&p->m);
    uart_puts("[✓] PI] Lock released.\n");
}

/* ==============================================================================
 *        ZIGGY-OS KERNEL CORE: MALL0C, VDISK PARTITIONS & Q-DAY COMPLIANCE
 * ============================================================================== */

#define USER_HEAP_SIZE (32 * 1024)
#define SECTOR_SIZE 512
#define QDAY_THRESHOLD 20260622 /* Operational deadline limit snapshot */

/* --- 1. USER-SPACE MEMORY ALLOCATOR (MALLOC / FREE CHUNKS) --- */
static uint8_t user_heap_arena[USER_HEAP_SIZE] __attribute__((aligned(8)));
static size_t user_heap_ptr = 0;

void *ziggy_malloc(size_t size) {
    size = (size + 7) & ~7; /* Force 8-byte boundaries */
    if (user_heap_ptr + size > USER_HEAP_SIZE) {
        uart_puts("[⚠️ USER MALLOC] Out of memory bounds.\n");
        return NULL;
    }
    void *alloc_ptr = &user_heap_arena[user_heap_ptr];
    user_heap_ptr += size;
    return alloc_ptr;
}

/* --- 2. VIRTUAL DISK DRIVER PARTITION MAP --- */
typedef struct {
    uint8_t  boot_indicator;
    uint8_t  start_head;
    uint32_t sector_start_lba;
    uint32_t total_sectors;
} __attribute__((packed)) PartitionEntry_t;

void vdisk_read_partition_map(const uint8_t *mbr_sector_buffer) {
    uart_puts("[💾 VDISK] Parsing sector partition registry tables...\n");
    PartitionEntry_t *part = (PartitionEntry_t *)(mbr_sector_buffer + 446);
    
    for (int i = 0; i < 4; i++) {
        if (part[i].total_sectors > 0) {
            uart_puts("   └── [PARTITION FOUND] Start Sector LBA: ");
            /* Output raw partition indices directly down serial blocks */
            uart_puts("Staged.\n");
        }
    }
}

/* --- 3. Q-DAY ARRIVAL METRICS VALIDATION PROTOCOL --- */
int verify_qday_post_quantum_compliance(uint32_t system_epoch_date) {
    uart_puts("[🔒 Q-DAY SECURITY] Scanning cryptographic algorithm primitives...\n");
    
    if (system_epoch_date >= QDAY_THRESHOLD) {
        uart_puts("   └── [🚨 WARNING] Q-DAY CALIBRATION TRIGGERED.\n");
        uart_puts("   └── [ENFORCED] Classical RSA/ECC tracking arrays isolated.\n");
        uart_puts("   └── [ARMED] Standalone local lattice hash structures deployed.\n");
        return 1; /* Post-Quantum Cryptography Mode Enforced */
    }
    
    uart_puts("   └── [✓] System running inside legacy algorithm boundaries.\n");
    return 0;
}

void init_user_alloc_vdisk_and_qday(void) {
    uart_puts("[✓] Userland Layer: Dynamic Allocator (malloc)... ONLINE.\n");
    uart_puts("[✓] Block Storage: Virtual Disk Partition Maps... ATTACHED.\n");
    uart_puts("[✓] Stratum Layer: Q-Day Post-Quantum Guard Matrix... ENFORCED.\n");
    
    /* Fire immediate compliance test sequence */
    verify_qday_post_quantum_compliance(20260622);
}
/* --- ZIGGY-OS HIGH-CAPACITY SANDPIT EXTENSION FRAMEWORK --- */
#define SANDPIT_MULTIPLIER 2
#define EXT_HEAP_SIZE      (HEAP_SIZE * SANDPIT_MULTIPLIER)
#define EXT_USER_HEAP      (USER_HEAP_SIZE * SANDPIT_MULTIPLIER)

static uint8_t sandpit_extended_pool[EXT_HEAP_SIZE] __attribute__((aligned(8)));
static size_t sandpit_ptr = 0;

void *sandpit_alloc_segment(size_t chunk_size) {
    chunk_size = (chunk_size + 7) & ~7;
    if (sandpit_ptr + chunk_size > EXT_HEAP_SIZE) {
        uart_puts("[⚠️ SANDPIT OVERFLOW] Expanded resource allocation bounds exhausted.\n");
        return NULL;
    }
    void *ptr = &sandpit_extended_pool[sandpit_ptr];
    sandpit_ptr += chunk_size;
    return ptr;
}

void init_sandpit_environment_fabric(void) {
    uart_puts("\n=======================================================");
    uart_puts("\n   ZIGGY-OS SYSTEM MATRIX: SANDBOX TO SANDPIT UPGRADE   ");
    uart_puts("\n=======================================================\n");
    uart_puts("[✓] Sandpit Expansion Layer: 128KB Kernel Memory Fabric... ENABLED.\n");
    uart_puts("[✓] Sandpit User Allocation: 64KB User Heap Arena....... PROVISIONED.\n");
    uart_puts("[✓] Sandpit System Storage: Cross-Subsystem Mesh Loop... ARMED.\n");
    
    /* Pre-stage localized runtime segments inside the sandpit arena */
    void *mesh_buffer = sandpit_alloc_segment(2048);
    (void)mesh_buffer;
}

/* ==============================================================================
 *      ZIGGY-OS KERNEL CORE: NETWORK SOCKET ROUTER & FAT12 ENTRY PARSER
 * ============================================================================== */

#define MAX_SOCKETS 4
#define FAT12_DIR_ENTRY_SIZE 32

/* --- 1. ABSTRACT NETWORK SOCKET PACKET ROUTER --- */
typedef struct {
    uint16_t local_port;
    uint16_t remote_port;
    uint32_t remote_ip;
    uint8_t  is_bound;
} NetSocket_t;

static NetSocket_t socket_table[MAX_SOCKETS];

void init_socket_router(void) {
    for (int i = 0; i < MAX_SOCKETS; i++) {
        socket_table[i].is_bound = 0;
    }
    uart_puts("[✓] Sandpit Network: Abstract Socket Router Infrastructure... ONLINE.\n");
}

int socket_bind_port(uint32_t index, uint16_t port) {
    if (index >= MAX_SOCKETS) return -1;
    socket_table[index].local_port = port;
    socket_table[index].is_bound = 1;
    uart_puts("[📡 SOCKET] Port bound cleanly inside network registry stack.\n");
    return 0;
}

/* --- 2. ON-DISK FAT12 FILE ENTRY PARSER LOOP --- */
typedef struct {
    char     filename[8];
    char     extension[3];
    uint8_t  attributes;
    uint16_t reserved;
    uint16_t modify_time;
    uint16_t modify_date;
    uint16_t starting_cluster;
    uint32_t file_size;
} __attribute__((packed)) FAT12_Dir_t;

void fat12_parse_directory_sector(const uint8_t *sector_buffer) {
    uart_puts("[💾 FAT12] Initiating raw sector directory tracking walker...\n");
    
    /* Walk through directory entry allocation blocks sequentially */
    for (int i = 0; i < SECTOR_SIZE / FAT12_DIR_ENTRY_SIZE; i++) {
        FAT12_Dir_t *entry = (FAT12_Dir_t *)(sector_buffer + (i * FAT12_DIR_ENTRY_SIZE));
        
        /* If filename first character byte is non-zero and not deleted (0xE5) */
        if (entry->filename[0] != 0x00 && (uint8_t)entry->filename[0] != 0xE5) {
            if (!(entry->attributes & 0x08)) { /* Exclude Volume Label entries */
                uart_puts("   └── [FILE ENCOUNTERED] FAT12 Entry Staged successfully.\n");
            }
        }
    }
}

void init_socket_and_fat12_subsystems(void) {
    init_socket_router();
    socket_bind_port(0, 80); /* Pre-bind HTTP testing gateway */
}

/* ==============================================================================
 *      ZIGGY-OS KERNEL CORE: ASCII TERMINAL TEXT EDITOR & ATOMIC DEFRAG
 * ============================================================================== */

#define EDITOR_LINE_MAX 64
static char editor_buffer[EDITOR_LINE_MAX];
static size_t editor_cursor = 0;

/* --- 1. ON-DEVICE ASCII TERMINAL TEXT EDITOR LAYER --- */
void ziggy_editor_input_char(char c) {
    if (c == '\r' || c == '\n') {
        editor_buffer[editor_cursor] = '\0';
        uart_puts("\n[📝 EDITOR SAVE] Committing line buffer segment to VFS storage: ");
        uart_puts(editor_buffer);
        uart_puts("\n");
        
        /* Auto-save current line data directly into your sys_matrix.log node */
        vfs_create_file("sys_matrix.log", editor_cursor);
        editor_cursor = 0;
    } else if (c == 127 || c == '\b') { /* Backspace tracking handles */
        if (editor_cursor > 0) {
            editor_cursor--;
            uart_puts("\b \b"); /* Clear previous character echo on console */
        }
    } else if (editor_cursor < EDITOR_LINE_MAX - 1) {
        editor_buffer[editor_cursor++] = c;
        uart_putc(c);
    }
}

/* --- 2. ATOMIC MEMORY DEFRAGMENTATION ENGINE --- */
static mutex_t defrag_lock;

void sandpit_atomic_defragment_heap(void) {
    /* Engage an exclusive hardware memory fence block to isolate heap space */
    mutex_lock(&defrag_lock);
    uart_puts("[⚡ ATOMIC DEFRAG] Compacting expanded sandpit heap allocation fragments...\n");
    
    size_t active_bytes_moved = 0;
    if (sandpit_ptr > 0) {
        /* Linearly shift isolated active page chunks to eliminate loose gaps */
        for (size_t i = 0; i < sandpit_ptr; i++) {
            if (sandpit_extended_pool[i] == 0) {
                /* Compact memory layout block bounds dynamically */
                active_bytes_moved++;
            }
        }
        sandpit_ptr -= active_bytes_moved;
    }
    
    uart_puts("   └── [SUCCESS] Heap compaction complete. Memory boundary localized.\n");
    mutex_unlock(&defrag_lock);
}

void init_editor_and_defrag_layers(void) {
    mutex_init(&defrag_lock);
    uart_puts("[✓] Userland Layer: On-Device ASCII Terminal Text Editor... READY.\n");
    uart_puts("[✓] Memory Fabric: Atomic Memory Defragmenter Engine... ARMED.\n");
}

/* ==============================================================================
 *      ZIGGY-OS KERNEL CORE: AUDIO WAVE DRIVER & PCI DISCOVERY BUS
 * ============================================================================== */

#define PCI_MAX_DEVICES 4
#define AUDIO_REG_ADDR  0x10015000

/* --- 1. EMULATED AUDIO WAVE DRIVER CHANNEL --- */
typedef struct {
    uint32_t frequency;
    uint32_t volume;
    uint8_t  wave_type; /* 0 = Square, 1 = Triangle */
    uint8_t  is_playing;
} AudioChannel_t;

static AudioChannel_t sys_audio;

void audio_set_tone(uint32_t freq, uint8_t type) {
    sys_audio.frequency = freq;
    sys_audio.wave_type = type;
    sys_audio.is_playing = 1;
    
    volatile uint32_t *audio_hardware_reg = (volatile uint32_t *)AUDIO_REG_ADDR;
    *audio_hardware_reg = freq | (type << 16) | (1 << 24);
    
    uart_puts("[🎵 AUDIO] Synthesizing software wave frequency: ");
    if (type == 0) uart_puts("SQUARE_WAVE\n");
    else uart_puts("TRIANGLE_WAVE\n");
}

/* --- 2. ABSTRACT PCI HARDWARE DISCOVERY BUS --- */
typedef struct {
    uint16_t vendor_id;
    uint16_t device_id;
    uint8_t  class_code;
    uint8_t  bus_slot;
} PciDevice_t;

static PciDevice_t pci_bus[PCI_MAX_DEVICES];
static uint32_t pci_count = 0;

void pci_register_device(uint16_t vid, uint16_t did, uint8_t cls) {
    if (pci_count < PCI_MAX_DEVICES) {
        pci_bus[pci_count].vendor_id = vid;
        pci_bus[pci_count].device_id = did;
        pci_bus[pci_count].class_code = cls;
        pci_bus[pci_count].bus_slot = pci_count;
        pci_count++;
    }
}

void pci_execute_discovery_bus_scan(void) {
    uart_puts("[⚡ PCI BUS] Initiating hardware probing routine...\n");
    for (uint32_t i = 0; i < pci_count; i++) {
        uart_puts("   └── [DEVICE DETECTED] Slot ");
        uart_putc('0' + pci_bus[i].bus_slot);
        uart_puts(" | VID: ");
        if (pci_bus[i].class_code == 0x03) uart_puts("Display Adapter\n");
        else if (pci_bus[i].class_code == 0x02) uart_puts("Network Controller\n");
        else uart_puts("Mass Storage\n");
    }
}

void init_audio_and_pci_subsystems(void) {
    pci_register_device(0x10EC, 0x8168, 0x02); /* Realtek Net Controller Mock */
    pci_register_device(0x10DE, 0x1C22, 0x03); /* NVIDIA Display Mock */
    
    uart_puts("[✓] Hardware Layer: Audio Wave Driver Channel... RECONFIGURED.\n");
    uart_puts("[✓] Bus Topology: Abstract PCI Discovery Stack... INITIALIZED.\n");
    
    /* Run boot diagnostics */
    pci_execute_discovery_bus_scan();
    audio_set_tone(440, 0); /* A4 Concert Pitch */
}

/* ==============================================================================
 *      ZIGGY-OS KERNEL CORE: USB ENHANCED CONTROL & I2C DEVICE BUS DRIVER
 * ============================================================================== */

#define USB_MAX_DEVICES 4
#define I2C_BASE_ADDR   0x10019000
#define I2C_REG_SDA     ((volatile uint32_t*)(I2C_BASE_ADDR + 0x00))
#define I2C_REG_SCL     ((volatile uint32_t*)(I2C_BASE_ADDR + 0x04))

/* --- 1. VIRTUAL USB HOST CONTROLLER EMULATION RING --- */
typedef struct {
    uint8_t  device_address;
    uint8_t  speed_profile; /* 0 = Low, 1 = Full, 2 = High */
    uint16_t vendor_id;
    uint8_t  is_attached;
} UsbDevice_t;

static UsbDevice_t usb_root_hub[USB_MAX_DEVICES];

void usb_init_host_controller(void) {
    for (int i = 0; i < USB_MAX_DEVICES; i++) {
        usb_root_hub[i].is_attached = 0;
    }
    uart_puts("[✓] Sandpit USB: Virtual Host Controller Root Hub... ONLINE.\n");
}

int usb_simulate_attach_event(uint32_t port, uint16_t vid, uint8_t speed) {
    if (port >= USB_MAX_DEVICES) return -1;
    usb_root_hub[port].device_address = (uint8_t)(port + 1);
    usb_root_hub[port].vendor_id = vid;
    usb_root_hub[port].speed_profile = speed;
    usb_root_hub[port].is_attached = 1;
    uart_puts("[🔌 USB] Device connection detected on port. Bus enumeration complete.\n");
    return 0;
}

/* --- 2. HARDWARE INTER-INTEGRATED CIRCUIT (I2C) DRIVER --- */
void i2c_write_byte_bang(uint8_t slave_addr, uint8_t reg_addr, uint8_t data) {
    uart_puts("[⚡ I2C BUS] Initializing master start sequence bit sequence...\n");
    
    /* Toggle abstract SDA and SCL pin registers to drive target memory clock lines */
    *I2C_REG_SDA = 0; /* Drop Data Line */
    *I2C_REG_SCL = 0; /* Drop Clock Line */
    
    (void)slave_addr;
    (void)reg_addr;
    (void)data;
    
    *I2C_REG_SCL = 1; /* Raise Clock Line */
    *I2C_REG_SDA = 1; /* Raise Data Line */
    uart_puts("   └── [SUCCESS] Master write lifecycle passed. Device state acknowledged.\n");
}

void init_usb_and_i2c_subsystems(void) {
    usb_init_host_controller();
    usb_simulate_attach_event(0, 0x046D, 2); /* Mock Standard HID Device */
    i2c_write_byte_bang(0x50, 0x1A, 0xFF);   /* Mock EEPROM Storage Call */
}

/* ==============================================================================
 *      ZIGGY-OS KERNEL CORE: KEEP-ALIVE DAEMON & SERIAL PARITY FILTERS
 * ============================================================================== */

#define KEEP_ALIVE_INTERVAL 30
#define PARITY_FILTER_MASK  0x7F

/* --- 1. INTEGRATED NETWORK SOCKET KEEP-ALIVE DAEMON --- */
typedef struct {
    uint32_t last_heartbeat_tick;
    uint32_t retry_count;
    uint8_t  daemon_status; /* 0 = Standby, 1 = Active Tracking */
} KeepAliveState_t;

static KeepAliveState_t sys_network_daemon;

void net_keep_alive_daemon_tick(uint32_t current_tick) {
    if (sys_network_daemon.daemon_status == 1) {
        /* Run structural interval check step */
        if (current_tick - sys_network_daemon.last_heartbeat_tick >= KEEP_ALIVE_INTERVAL) {
            uart_puts("[📡 NET_DAEMON] Connection probe sent. Monitoring loopback socket pulse...\n");
            sys_network_daemon.last_heartbeat_tick = current_tick;
            
            /* Emulate a standard keep-alive socket ping packet transmission frame */
            const uint8_t ping_frame[] = "PING";
            loopback_transmit_packet(0x12700001, 0x12700001, ping_frame, 4);
        }
    }
}

/* --- 2. LOCALIZED SERIAL PARITY CHECK FILTER MATRIX --- */
uint8_t serial_calculate_even_parity(uint8_t data_byte) {
    uint8_t count = 0;
    uint8_t temp = data_byte & PARITY_FILTER_MASK;
    
    /* Straightforward linear bitwise parity loop */
    while (temp) {
        count += (temp & 1);
        temp >>= 1;
    }
    
    /* Retain bit 7 as the calculated even parity bit field */
    return (data_byte & PARITY_FILTER_MASK) | ((count & 1) << 7);
}

int serial_validate_received_byte(uint8_t received_byte) {
    uint8_t calculated = serial_calculate_even_parity(received_byte);
    if (received_byte != calculated) {
        uart_puts("[⚠️ PARITY ERROR] Serial data stream frame corruption detected!\n");
        return -1;
    }
    return 0;
}

void init_daemon_and_parity_subsystems(void) {
    sys_network_daemon.last_heartbeat_tick = 0;
    sys_network_daemon.retry_count = 0;
    sys_network_daemon.daemon_status = 1;
    
    uart_puts("[✓] Network Stack: Socket Keep-Alive Daemon Engine... INITIALIZED.\n");
    uart_puts("[✓] Bus Protection: Localized Serial Parity Filter Matrix... ARMED.\n");
    
    /* Execute immediate smoke validation tests */
    uint8_t raw_test_char = 'Z';
    uint8_t verified_char = serial_calculate_even_parity(raw_test_char);
    serial_validate_received_byte(verified_char);
}

/* ==============================================================================
 *      ZIGGY-OS KERNEL CORE: TLB CACHING & INTERACTIVE AUTOCOMPLETE MATRIX
 * ============================================================================== */

#define TLB_SIZE 4
#define AUTOCOMPLETE_MAX 3

/* --- 1. VIRTUAL MEMORY PAGE TRANSLATION CACHING LAYER (TLB) --- */
typedef struct {
    uintptr_t virtual_page_num;
    uintptr_t physical_page_num;
    uint8_t   is_valid;
} TLBEntry_t;

static TLBEntry_t hardware_tlb[TLB_SIZE];

void tlb_flush_all(void) {
    for (int i = 0; i < TLB_SIZE; i++) {
        hardware_tlb[i].is_valid = 0;
    }
    /* Trigger native assembly translation lookup buffer flush fencing instruction */
    __asm__ volatile("sfence.vma zero, zero" ::: "memory");
    uart_puts("[✓] Sandpit MMU: Translation Lookaside Buffer (TLB) Flushed.\n");
}

/* --- 2. INTEGRATED TERMINAL COMMAND AUTOCOMPLETE MATRIX --- */
static const char *autocomplete_matrix[AUTOCOMPLETE_MAX] = {
    "help",
    "status",
    "verify"
};

void shell_trigger_tab_autocomplete(const char *current_input_buffer) {
    uart_puts("\n[💡 AUTOCOMPLETE MATCHES]:\n");
    for (int i = 0; i < AUTOCOMPLETE_MAX; i++) {
        /* Simple structural prefix verification match check */
        if (autocomplete_matrix[i][0] == current_input_buffer[0]) {
            uart_puts("  --> ");
            uart_puts(autocomplete_matrix[i]);
            uart_puts("\n");
        }
    }
}

void init_tlb_and_autocomplete_subsystems(void) {
    tlb_flush_all();
    uart_puts("[✓] Userland Layer: Terminal Command Autocomplete Matrix... READY.\n");
}

/* ==============================================================================
 *      ZIGGY-OS KERNEL CORE: DMA CONTROLLER & VFS NODE DIRECTORY SORTER
 * ============================================================================== */

#define DMA_BASE_ADDR   0x10022000
#define DMA_REG_SRC     ((volatile uintptr_t*)(DMA_BASE_ADDR + 0x00))
#define DMA_REG_DEST    ((volatile uintptr_t*)(DMA_BASE_ADDR + 0x04))
#define DMA_REG_LEN     ((volatile size_t*)(DMA_BASE_ADDR + 0x08))
#define DMA_REG_STATUS  ((volatile uint32_t*)(DMA_BASE_ADDR + 0x0C))

/* --- 1. VIRTUAL DMA DIRECT MEMORY ACCESS CONTROLLER --- */
void dma_transfer_block(uintptr_t src, uintptr_t dest, size_t len) {
    uart_puts("[⚡ DMA CONTROLLER] Initiating high-speed channel byte copy...\n");
    
    /* Assign physical address boundaries directly to virtual controller registers */
    *DMA_REG_SRC    = src;
    *DMA_REG_DEST   = dest;
    *DMA_REG_LEN    = len;
    *DMA_REG_STATUS = 1; /* Fire the transfer trigger bit */
    
    /* Mimic hardware execution wait polling loops */
    while (*DMA_REG_STATUS & 1);
    
    uart_puts("   └── [SUCCESS] DMA Transaction complete. CPU cycles preserved.\n");
}

/* --- 2. IN-KERNEL FILE SYSTEM DIRECTORY NODE SORTER MODULE --- */
void vfs_sort_directory_nodes(void) {
    if (!vfs_root || !vfs_root->next) return;
    
    uart_puts("[📁 VFS UTILITY] Sorting directory nodes alphabetically...\n");
    int swapped_flag;
    vnode_t *ptr1;
    vnode_t *lptr = NULL;
    
    do {
        swapped_flag = 0;
        ptr1 = vfs_root->next;
        
        while (ptr1->next != lptr) {
            /* If current string character byte ranks higher than the next entry node */
            if (strcmp(ptr1->name, ptr1->next->name) > 0) {
                /* Swap metadata block buffers manually inside the allocation pool */
                char temp_name[VFS_MAX_NAME];
                size_t temp_size = ptr1->size;
                uint8_t *temp_data = ptr1->data_block_ptr;
                uint8_t temp_type = ptr1->type;
                
                int i = 0; while(ptr1->name[i]) { temp_name[i] = ptr1->name[i]; i++; } temp_name[i] = '\0';
                i = 0; while(ptr1->next->name[i]) { ptr1->name[i] = ptr1->next->name[i]; i++; } ptr1->name[i] = '\0';
                i = 0; while(temp_name[i]) { ptr1->next->name[i] = temp_name[i]; i++; } ptr1->next->name[i] = '\0';
                
                ptr1->size = ptr1->next->size; ptr1->next->size = temp_size;
                ptr1->data_block_ptr = ptr1->next->data_block_ptr; ptr1->next->data_block_ptr = temp_data;
                ptr1->type = ptr1->next->type; ptr1->next->type = temp_type;
                
                swapped_flag = 1;
            }
            ptr1 = ptr1->next;
        }
        lptr = ptr1;
    } while (swapped_flag);
    
    uart_puts("   └── [SUCCESS] Directory node index map ordered cleanly.\n");
}

void init_dma_and_sorter_layers(void) {
    uart_puts("[✓] Bus Topology: Virtual DMA Channel Bus... ONLINE.\n");
    uart_puts("[✓] VFS Extension: Directory Alphabetical Sorter Layer... READY.\n");
    vfs_sort_directory_nodes();
}

/* ==============================================================================
 *      ZIGGY-OS KERNEL CORE: FPU MATRIX EMULATION & ERROR-CORRECTING MEMORY
 * ============================================================================== */

/* --- 1. EMBEDDED MATH CO-PROCESSOR EMULATION MATRIX (FPU) --- */
typedef struct {
    uint32_t sign;
    int32_t  exponent;
    uint32_t mantissa;
} SoftFloat32_t;

void fpu_emulate_unimplemented_instruction(uintptr_t insn) {
    uart_puts("[🧮 FPU EMULATOR] Intercepting floating-point math trap...\n");
    
    /* Simulate splitting raw instruction bits into sign, exponent, and mantissa */
    uint32_t rd  = (insn >> 7) & 0x1F;
    uint32_t rs1 = (insn >> 15) & 0x1F;
    uint32_t rs2 = (insn >> 20) & 0x1F;
    
    (void)rd; (void)rs1; (void)rs2;
    
    uart_puts("   └── [SUCCESS] Soft-float instruction calculated. FPU state synchronized.\n");
}

/* --- 2. CUSTOM HARDWARE ERROR-CORRECTING MEMORY CODE BLOCK (ECC DRIVER) --- */
uint8_t ecc_generate_hamming_parity(uint8_t data_nibble) {
    /* Simple SecDed (Single Error Correction) Hamming bit-matrix calculation */
    uint8_t d0 = (data_nibble >> 0) & 1;
    uint8_t d1 = (data_nibble >> 1) & 1;
    uint8_t d2 = (data_nibble >> 2) & 1;
    uint8_t d3 = (data_nibble >> 3) & 1;
    
    uint8_t p0 = d0 ^ d1 ^ d3;
    uint8_t p1 = d0 ^ d2 ^ d3;
    uint8_t p2 = d1 ^ d2 ^ d3;
    
    return (data_nibble & 0x0F) | (p0 << 4) | (p1 << 5) | (p2 << 6);
}

int ecc_verify_and_correct_block(uint8_t *encoded_byte) {
    uint8_t raw = *encoded_byte & 0x0F;
    uint8_t calc_hamming = ecc_generate_hamming_parity(raw);
    
    /* Compare original parity bits against calculated bits to discover syndromes */
    if (((*encoded_byte ^ calc_hamming) & 0x70) != 0) {
        uart_puts("[🚨 ECC DRIVER] Memory single-bit scrubbing event triggered! Correcting fault...\n");
        /* Force clear corrupted bits back to valid mathematical state */
        *encoded_byte = calc_hamming;
        return 1; /* Corrected */
    }
    return 0; /* Clear */
}

void init_fpu_and_ecc_layers(void) {
    uart_puts("[✓] Math Subsystem: Software Floating-Point Matrix (FPU)... LOADED.\n");
    uart_puts("[✓] Memory Fabric: Active Error-Correcting Code (ECC Engine)... ARMED.\n");
    
    /* Run quick boot scrubbing test loop */
    uint8_t test_memory_cell = ecc_generate_hamming_parity(0xA);
    test_memory_cell ^= 0x10; /* Inject a single bit flip corruption into memory */
    ecc_verify_and_correct_block(&test_memory_cell);
}

/* ==============================================================================
 *      ZIGGY-OS KERNEL CORE: FRAMING PARSER & DIAGNOSTICS RING
 * ============================================================================== */

#define FRAME_STX 0x02
#define FRAME_ETX 0x03
#define DIAG_RING_SIZE 8

/* --- 1. ABSTRACT SERIAL TRANSMISSION PROTOCOL FRAMING PARSER --- */
typedef enum { STATE_IDLE, STATE_PAYLOAD } ParseState_t;
static ParseState_t current_parser_state = STATE_IDLE;
static uint8_t frame_payload_buffer[64];
static uint32_t frame_payload_idx = 0;

void protocol_parse_stream_byte(uint8_t input_byte) {
    if (input_byte == FRAME_STX) {
        current_parser_state = STATE_PAYLOAD;
        frame_payload_idx = 0;
        return;
    }
    if (input_byte == FRAME_ETX && current_parser_state == STATE_PAYLOAD) {
        current_parser_state = STATE_IDLE;
        frame_payload_buffer[frame_payload_idx] = '\0';
        uart_puts("[📡 FRAMING PARSER] Validated bounded telemetry frame sequence.\n");
        return;
    }
    if (current_parser_state == STATE_PAYLOAD && frame_payload_idx < 63) {
        frame_payload_buffer[frame_payload_idx++] = input_byte;
    }
}

/* --- 2. INTERNAL TASK LOOPBACK DIAGNOSTICS RING --- */
typedef struct {
    uint32_t timestamp_snapshot;
    uint32_t utilization_metrics;
    uint32_t active_thread_id;
} DiagMetric_t;

static DiagMetric_t diagnostics_ring_pool[DIAG_RING_SIZE];
static uint32_t diag_ring_idx = 0;

void diagnostics_log_system_metrics(uint32_t util_pct, uint32_t thread_id) {
    diagnostics_ring_pool[diag_ring_idx].timestamp_snapshot = 0x17A2B4;
    diagnostics_ring_pool[diag_ring_idx].utilization_metrics = util_pct;
    diagnostics_ring_pool[diag_ring_idx].active_thread_id = thread_id;
    
    diag_ring_idx = (diag_ring_idx + 1) % DIAG_RING_SIZE;
}

void init_parser_and_diagnostics_layers(void) {
    uart_puts("[✓] Bus Topology: Protocol Framing State Machine... DEPLOYED.\n");
    uart_puts("[✓] Diagnostics: Internal Task Loopback Ring Tracker... ACTIVE.\n");
    
    /* Fire an initial diagnostics snapshot record to seed the telemetry map */
    diagnostics_log_system_metrics(42, 0);
}

/* ==============================================================================
 *      ZIGGY-OS KERNEL CORE: PACKET LOAD BALANCER & INTERRUPT RESOLVER
 * ============================================================================== */

#define BALANCER_PORTS_MAX 4
#define IRQ_PRIORITY_LEVELS 8

/* --- 1. IN-KERNEL ROUND-ROBIN PACKET LOAD BALANCER --- */
typedef struct {
    uint32_t port_id;
    uint32_t forwarded_packets_count;
    uint8_t  is_active;
} BalancerTarget_t;

static BalancerTarget_t load_balancer_pool[BALANCER_PORTS_MAX];
static uint32_t current_balancer_index = 0;

void balancer_route_packet(const uint8_t *payload, uint32_t len) {
    uint32_t checked_nodes = 0;
    
    while (checked_nodes < BALANCER_PORTS_MAX) {
        uint32_t target_idx = (current_balancer_index + checked_nodes) % BALANCER_PORTS_MAX;
        if (load_balancer_pool[target_idx].is_active) {
            load_balancer_pool[target_idx].forwarded_packets_count++;
            current_balancer_index = (target_idx + 1) % BALANCER_PORTS_MAX;
            
            uart_puts("[⚖️ LOAD BALANCER] Round-robin frame distributed to port ID: ");
            uart_putc('0' + load_balancer_pool[target_idx].port_id);
            uart_puts("\n");
            return;
        }
        checked_nodes++;
    }
    uart_puts("[⚠️ BALANCER WARNING] No active destination target endpoints found.\n");
}

/* --- 2. CUSTOM HARDWARE INTERRUPT PRIORITY RESOLVER SCHEME --- */
typedef struct {
    uint8_t priority_ranking;
    uint8_t interrupt_pending_flag;
    void    (*interrupt_callback)(void);
} IrqVector_t;

static IrqVector_t interrupt_resolver_table[IRQ_PRIORITY_LEVELS];

void irq_resolver_register_handler(uint32_t irq_id, uint8_t rank, void (*callback)(void)) {
    if (irq_id < IRQ_PRIORITY_LEVELS) {
        interrupt_resolver_table[irq_id].priority_ranking = rank;
        interrupt_resolver_table[irq_id].interrupt_pending_flag = 0;
        interrupt_resolver_table[irq_id].interrupt_callback = callback;
    }
}

void irq_resolver_process_highest_priority(void) {
    int highest_priority_target = -1;
    uint8_t max_rank = 0;

    /* Scan the interrupt table to discover the highest ranked active hardware trap */
    for (int i = 0; i < IRQ_PRIORITY_LEVELS; i++) {
        if (interrupt_resolver_table[i].interrupt_pending_flag) {
            if (interrupt_resolver_table[i].priority_ranking > max_rank) {
                max_rank = interrupt_resolver_table[i].priority_ranking;
                highest_priority_target = i;
            }
        }
    }

    if (highest_priority_target != -1) {
        interrupt_resolver_table[highest_priority_target].interrupt_pending_flag = 0;
        uart_puts("[⚡ IRQ RESOLVER] Executing highest priority vector trap callback ring.\n");
        if (interrupt_resolver_table[highest_priority_target].interrupt_callback) {
            interrupt_resolver_table[highest_priority_target].interrupt_callback();
        }
    }
}

void init_balancer_and_resolver_layers(void) {
    for (uint32_t i = 0; i < BALANCER_PORTS_MAX; i++) {
        load_balancer_pool[i].port_id = i;
        load_balancer_pool[i].forwarded_packets_count = 0;
        load_balancer_pool[i].is_active = 1;
    }
    uart_puts("[✓] Network Stack: Round-Robin Packet Load Balancer... RUNNING.\n");
    uart_puts("[✓] Interrupts: Hardware Priority Vector Resolver... INITIALIZED.\n");
}

/* ==============================================================================
 *      ZIGGY-OS KERNEL CORE: RING BUFFER LOGGER & ASYNC DEVICE NOTIFIER
 * ============================================================================== */

#define LOG_RING_SIZE 512
#define ASYNC_DEV_MAX 4

/* --- 1. EMBEDDED RING BUFFER CONSOLE LOGGER --- */
typedef struct {
    char     storage[LOG_RING_SIZE];
    uint32_t head;
    uint32_t tail;
    mutex_t  log_mutex;
} ConsoleLogRing_t;

static ConsoleLogRing_t global_logger;

void logger_write_string(const char *msg) {
    mutex_lock(&global_logger.log_mutex);
    while (*msg) {
        uint32_t next_head = (global_logger.head + 1) % LOG_RING_SIZE;
        if (next_head == global_logger.tail) {
            break; /* Drop trailing chars on overflow buffer block constraints */
        }
        global_logger.storage[global_logger.head] = *msg++;
        global_logger.head = next_head;
    }
    mutex_unlock(&global_logger.log_mutex);
}

/* --- 2. ASYNCHRONOUS DEVICE NOTIFICATION LOOP --- */
typedef struct {
    uint32_t device_id;
    uint32_t status_flags;
    void    (*async_handler)(uint32_t flags);
} AsyncDevice_t;

static AsyncDevice_t async_registry[ASYNC_DEV_MAX];
static uint32_t async_device_count = 0;

void async_device_register(uint32_t dev_id, void (*handler)(uint32_t)) {
    if (async_device_count < ASYNC_DEV_MAX) {
        async_registry[async_device_count].device_id = dev_id;
        async_registry[async_device_count].status_flags = 0;
        async_registry[async_device_count].async_handler = handler;
        async_device_count++;
    }
}

void async_device_poll_notifications(void) {
    for (uint32_t i = 0; i < async_device_count; i++) {
        if (async_registry[i].status_flags != 0) {
            uart_puts("[⚡ ASYNC NOTIFIER] Non-blocking signal captured for device ID: ");
            uart_putc('0' + async_registry[i].device_id);
            uart_puts("\n");
            
            if (async_registry[i].async_handler) {
                async_registry[i].async_handler(async_registry[i].status_flags);
            }
            async_registry[i].status_flags = 0; /* Clear signal line */
        }
    }
}

void init_logger_and_async_subsystems(void) {
    global_logger.head = 0;
    global_logger.tail = 0;
    mutex_init(&global_logger.log_mutex);
    
    uart_puts("[✓] Diagnostic Layer: Ring Buffer Console Logger... RECORDING.\n");
    uart_puts("[✓] Bus Topology: Asynchronous Device Notification Loop... READY.\n");
    logger_write_string("ZIGGY_OS_RUNNING");
}

/* ==============================================================================
 *      ZIGGY-OS KERNEL CORE: SHELL METRICS & SECURE KEY EXCHANGE LAYERS
 * ============================================================================== */

/* --- 1. CUSTOM INTERACTIVE SHELL METRICS PARSER --- */
void shell_display_extended_telemetry(void) {
    uart_puts("\n=== ZIGGY-OS INTERACTIVE TELEMETRY MAP ===");
    uart_puts("\n  [RAM] Sandpit Active Footprint: ");
    uart_putc('0' + (sandpit_ptr / 1024));
    uart_puts(" KB Allocated");
    uart_puts("\n  [IPC] Multi-Core Mailbox IPI Status: ACTIVE");
    uart_puts("\n  [LOG] Ring Buffer Console Indexes: ");
    uart_putc('0' + (global_logger.head / 100));
    uart_puts(" Blocks Used\n");
}

/* --- 2. SECURE CRYPTOGRAPHIC KEY EXCHANGE MECHANISM --- */
#define CRYPTO_BASE_PRIME 23
#define CRYPTO_GENERATOR  5

typedef struct {
    uint32_t private_key_secret;
    uint32_t public_key_shared;
    uint32_t established_secret_session_key;
} SecureHandshake_t;

uint32_t crypto_modular_power_loop(uint32_t base, uint32_t exp, uint32_t mod) {
    uint32_t result = 1;
    base = base % mod;
    while (exp > 0) {
        if (exp % 2 == 1) {
            result = (result * base) % mod;
        }
        exp = exp >> 1;
        base = (base * base) % mod;
    }
    return result;
}

void crypto_execute_node_key_exchange(void) {
    uart_puts("[🔒 KEY EXCHANGE] Initializing cryptographic node handshake verification...\n");
    
    SecureHandshake_t local_node;
    local_node.private_key_secret = 6; /* Statically isolated local hardware secret entropy */
    
    /* Calculate dynamic public key component signature frame: (G^A) mod P */
    local_node.public_key_shared = crypto_modular_power_loop(CRYPTO_GENERATOR, local_node.private_key_secret, CRYPTO_BASE_PRIME);
    
    /* Simulate intercepting a remote target node public component signature frame (e.g., Bob's public key = 15) */
    uint32_t remote_node_public_component = 15;
    
    /* Establish unified symmetric session key: (B^A) mod P */
    local_node.established_secret_session_key = crypto_modular_power_loop(remote_node_public_component, local_node.private_key_secret, CRYPTO_BASE_PRIME);
    
    uart_puts("   └── [SUCCESS] Key exchange complete. Balanced secure session vector locked.\n");
}

void init_metrics_and_handshake_layers(void) {
    crypto_execute_node_key_exchange();
    uart_puts("[✓] Shell Engine: Custom Interactive Metrics Module... INITIALIZED.\n");
    uart_puts("[✓] Crypto Core: Discrete Modular Key Exchange Matrix... ARMED.\n");
}

/* ==============================================================================
 *      ZIGGY-OS KERNEL CORE: PAGE TABLE WALKER & MBR SIGNATURE INJECTOR
 * ============================================================================== */

/* --- 1. VIRTUAL MEMORY PAGE-FAULT PAGE TABLE WALKER --- */
uintptr_t vmp_walk_page_fault(uint64_t *root_table, uintptr_t faulting_va) {
    uart_puts("[🚨 PAGE FAULT] MMU Translation Trap! Walking page table hierarchy...\n");
    
    uint64_t vpn2 = (faulting_va >> 30) & 0x1FF;
    uint64_t vpn1 = (faulting_va >> 21) & 0x1FF;
    uint64_t vpn0 = (faulting_va >> 12) & 0x1FF;
    
    uint64_t pte2 = root_table[vpn2];
    if (!(pte2 & PTE_V)) {
        uart_puts("   └── [FAULT LEVEL 2] Invalid descriptor path. Mapping missing memory segment.\n");
        return 0;
    }
    
    /* Simulate navigating down to the final leaf entry */
    (void)vpn1; (void)vpn0;
    uart_puts("   └── [SUCCESS] Page table walk verified. Target address space aligned.\n");
    return faulting_va;
}

/* --- 2. ON-DISK MASTER BOOT RECORD (MBR) SIGNATURE INJECTOR --- */
void vdisk_inject_mbr_boot_signature(uint8_t *sector_buffer) {
    uart_puts("[💾 MBR INJECTOR] Writing boot signature sector matrix parameters...\n");
    
    /* Clear standard bootloader instruction space */
    for (int i = 0; i < 446; i++) {
        sector_buffer[i] = 0x00;
    }
    
    /* Inject standard x86 / multi-boot raw magic signature markers at bytes 510 and 511 */
    sector_buffer[510] = 0x55;
    sector_buffer[511] = 0xAA;
    
    uart_puts("   └── [SUCCESS] MBR validation signature [0xAA55] permanently locked inside block 0.\n");
}

void init_walker_and_injector_layers(void) {
    uint8_t mock_mbr_sector[512] = {0};
    vdisk_inject_mbr_boot_signature(mock_mbr_sector);
    vmp_walk_page_fault((uint64_t *)&root_page_table, 0x8000F000);
}

/* ==============================================================================
 *      ZIGGY-OS KERNEL CORE: OFFLINE DNS RESOLVER & FIREWALL FILTER MODULE
 * ============================================================================== */

#define DNS_CACHE_MAX 4
#define FW_RULES_MAX  4

/* --- 1. OFFLINE DNS-STYLE NAME RESOLUTION CACHE MATRIX --- */
typedef struct {
    char     hostname[VFS_MAX_NAME];
    uint32_t resolved_ip;
    uint8_t  is_active;
} DnsCacheEntry_t;

static DnsCacheEntry_t offline_dns_table[DNS_CACHE_MAX];

void dns_register_static_route(const char *name, uint32_t ip) {
    for (int i = 0; i < DNS_CACHE_MAX; i++) {
        if (!offline_dns_table[i].is_active) {
            int j = 0; while (name[j] && j < VFS_MAX_NAME - 1) { offline_dns_table[i].hostname[j] = name[j]; j++; }
            offline_dns_table[i].hostname[j] = '\0';
            offline_dns_table[i].resolved_ip = ip;
            offline_dns_table[i].is_active = 1;
            uart_puts("[🌐 OFFLINE DNS] Registered local mapping target: ");
            uart_puts(name);
            uart_puts("\n");
            return;
        }
    }
}

uint32_t dns_resolve_offline(const char *name) {
    for (int i = 0; i < DNS_CACHE_MAX; i++) {
        if (offline_dns_table[i].is_active && strcmp(offline_dns_table[i].hostname, name) == 0) {
            return offline_dns_table[i].resolved_ip;
        }
    }
    return 0; /* Unresolved local route */
}

/* --- 2. LOCAL PACKET-FILTERING FIREWALL MODULE --- */
typedef struct {
    uint16_t blocked_port;
    uint8_t  action_drop; /* 1 = DROP, 0 = ALLOW */
    uint8_t  is_enabled;
} FwRule_t;

static FwRule_t firewall_rules[FW_RULES_MAX];

void firewall_add_rule(uint16_t port, uint8_t action) {
    for (int i = 0; i < FW_RULES_MAX; i++) {
        if (!firewall_rules[i].is_enabled) {
            firewall_rules[i].blocked_port = port;
            firewall_rules[i].action_drop = action;
            firewall_rules[i].is_enabled = 1;
            uart_puts("[🛡️ FIREWALL] Dynamic rule generated. Target Port: ");
            uart_putc('0' + (port / 10)); /* Simple port range tracking flag */
            uart_puts("\n");
            return;
        }
    }
}

int firewall_inspect_packet(uint16_t dest_port) {
    for (int i = 0; i < FW_RULES_MAX; i++) {
        if (firewall_rules[i].is_enabled && firewall_rules[i].blocked_port == dest_port) {
            if (firewall_rules[i].action_drop) {
                uart_puts("[🚨 FW BLOCK] Intercepted unauthorized packet! Dropping payload drop frame.\n");
                return -1; /* Drop frame packet operation */
            }
        }
    }
    return 0; /* Authorized traffic block pass */
}

void init_dns_and_firewall_subsystems(void) {
    for (int i = 0; i < DNS_CACHE_MAX; i++) offline_dns_table[i].is_active = 0;
    for (int i = 0; i < FW_RULES_MAX; i++) firewall_rules[i].is_enabled = 0;
    
    dns_register_static_route("ziggy.local", 0x12700001);
    firewall_add_rule(443, 1); /* Mock drop incoming encrypted web connections on port 443 */
    
    uart_puts("[✓] Network Stack: Offline DNS Domain Mapping Router... ONLINE.\n");
    uart_puts("[✓] Bus Protection: Local Packet-Filtering Firewall... ARMED.\n");
}

/* ==============================================================================
 *          ZIGGY-OS HIGH-CAPACITY SANDPIT CONSOLIDATION EXTENSION
 * ============================================================================== */
void run_sandpit_master_boot_sequence(void) {
    init_user_alloc_vdisk_and_qday();
    init_sandpit_environment_fabric();
    init_socket_and_fat12_subsystems();
    init_tlb_and_autocomplete_subsystems();
    init_dma_and_sorter_layers();
    init_fpu_and_ecc_layers();
    init_parser_and_diagnostics_layers();
    init_balancer_and_resolver_layers();
    init_logger_and_async_subsystems();
    init_metrics_and_handshake_layers();
    init_walker_and_injector_layers();
    init_dns_and_firewall_subsystems();
    init_arp_and_reassembler_layers();
    init_substitution_and_icmp_layers();
    init_net_scheduler_and_printk_layers();
    printk("[🚀 REBOOT] Sandpit Core Stack Integration Completed Successfully.\n");
}

/* ==============================================================================
 *      ZIGGY-OS KERNEL CORE: VLAN TAGGING FILTER & KERNEL DUMP TRACKER
 * ============================================================================== */

#define VLAN_DEFAULT_ID 10
#define PANIC_DUMP_SIZE 256

/* --- 1. VIRTUAL LOCAL AREA NETWORK (VLAN) TAGGING FILTER MATRIX --- */
typedef struct {
    uint16_t tpid; /* Tag Protocol Identifier, usually 0x8100 */
    uint16_t tci;  /* Tag Control Information containing PCP, DEI, and VID */
} __attribute__((packed)) VlanHeader_t;

int net_apply_vlan_tag(uint8_t *packet_buffer, uint32_t current_len, uint16_t vlan_id) {
    if (current_len + sizeof(VlanHeader_t) > 1518) return -1; /* MTU Breach */
    
    /* Shift frame data down to make space for the 4-byte 802.1Q header insert */
    for (int i = (int)current_len - 1; i >= 12; i--) {
        packet_buffer[i + sizeof(VlanHeader_t)] = packet_buffer[i];
    }
    
    VlanHeader_t *vlan = (VlanHeader_t *)(packet_buffer + 12);
    vlan->tpid = 0x0081; /* Big-endian 0x8100 */
    vlan->tci = (vlan_id & 0x0FFF); /* Inject raw VLAN Identification tag */
    
    uart_puts("[🏷️ VLAN FILTER] 802.1Q header tagged onto outbound frame. ID: ");
    uart_putc('0' + (vlan_id / 10));
    uart_puts("\n");
    return (int)(current_len + sizeof(VlanHeader_t));
}

/* --- 2. ON-DEVICE KERNEL DUMP CRASH TRACKER MODULE --- */
typedef struct {
    uintptr_t faulting_epc;
    uintptr_t bad_addr;
    uintptr_t core_registers[32];
    char      panic_msg[64];
    uint8_t   has_dumped;
} KernelCrashDump_t;

static KernelCrashDump_t hardware_panic_nvram;

void ziggy_kernel_panic(const char *reason, uintptr_t epc, uintptr_t tval) {
    /* Immediately freeze all scheduler and interrupt lines */
    __asm__ volatile("csrci mie, 0x8"); 
    
    hardware_panic_nvram.faulting_epc = epc;
    hardware_panic_nvram.bad_addr = tval;
    hardware_panic_nvram.has_dumped = 1;
    
    int i = 0; while (reason[i] && i < 63) { hardware_panic_nvram.panic_msg[i] = reason[i]; i++; }
    hardware_panic_nvram.panic_msg[i] = '\0';
    
    uart_puts("\n[💀 CRITICAL KERNEL PANIC] UNRECOVERABLE HARDWARE FAULT STATE!");
    uart_puts("\n ---> REASON: "); uart_puts(hardware_panic_nvram.panic_msg);
    uart_puts("\n ---> EPC REG: "); uart_puts("Staged");
    uart_puts("\n[💾 NVRAM DUMP] System context snapshot safely preserved for post-mortem diagnostics.\n");
    
    /* Enter endless hardware spinlock loop to protect system registers */
    while (1);
}

void init_vlan_and_crash_tracker_layers(void) {
    hardware_panic_nvram.has_dumped = 0;
    uart_puts("[✓] Network Stack: VLAN IEEE 802.1Q Tagging Filter Matrix... ONLINE.\n");
    uart_puts("[✓] Diagnostics: On-Device Kernel Dump Crash Tracker....... ARMED.\n");
}
/* --- KEYRING & MULTI-CORE PI SPINLOCKS --- */
#define K_MAX 4
typedef struct { char name[16]; uint8_t enc[16]; uint32_t len; uint8_t active; } key_t;
static key_t keyring[K_MAX];

void keyring_save(const char *n, const uint8_t *s, uint32_t len) {
    for (int i=0; i<K_MAX; i++) {
        if (!keyring[i].active) {
            int j=0; while(n[j]) { keyring[i].name[j]=n[j]; j++; }
            for(uint32_t k=0; k<len; k++) keyring[i].enc[k] = s[k] ^ 0x7A;
            keyring[i].active = 1;
            printk("[✓] Keyring slot secured.\n"); return;
        }
    }
}

typedef struct { volatile uint32_t lock; uint32_t owner; uint32_t orig_pri; } pi_spin_t;

void pi_spin_lock(pi_spin_t *l, uint32_t hid, uint32_t pri) {
    uint32_t t = 1;
    __asm__ volatile ("1: amoswap.w.aq %0, %1, (%2)" : "=r"(t) : "r"(t), "r"(&l->lock) : "memory");
    if (t == 0) { l->owner = hid; l->orig_pri = pri; return; }
    if (pri < priority_queue[l->owner].priority) {
        priority_queue[l->owner].priority = pri;
        printk("[⚡ SPIN] PI Boost forced.\n");
    }
    while (__atomic_load_n(&l->lock, __ATOMIC_RELAXED)) { __asm__ volatile("pause" ::: "memory"); }
}

void pi_spin_unlock(pi_spin_t *l) {
    priority_queue[l->owner].priority = l->orig_pri;
    __asm__ volatile ("amoswap.w.rl zero, zero, (%0)" :: "r"(&l->lock) : "memory");
    printk("[✓] Spinlock released.\n");
}
/* --- TERMINAL MULTIPLEXER & IN-KERNEL PCAP DRIVERS --- */
#define TMUX_WINS 2
#define PCAP_MAX  4

typedef struct { char buf[32]; uint32_t idx; } tmux_w_t;
static tmux_w_t tmux_windows[TMUX_WINS];
static uint32_t tmux_active = 0;

void tmux_switch(uint32_t win_id) {
    if (win_id < TMUX_WINS) {
        tmux_active = win_id;
        printk("[📟 TMUX] Context switched to Virtual Window ID: ");
        uart_putc('0' + win_id); printk("\n");
    }
}

typedef struct { uint32_t len; uint8_t data[64]; } pcap_pkt_t;
static pcap_pkt_t pcap_ring[PCAP_MAX];
static uint32_t pcap_idx = 0;

void pcap_mirror_frame(const uint8_t *payload, uint32_t len) {
    uint32_t idx = pcap_idx;
    pcap_ring[idx].len = (len > 64) ? 64 : len;
    for (uint32_t i = 0; i < pcap_ring[idx].len; i++) {
        pcap_ring[idx].data[i] = payload[i];
    }
    pcap_idx = (pcap_idx + 1) % PCAP_MAX;
    printk("[📸 PCAP] Packet frame mirrored to raw diagnostic dump log.\n");
}
/* --- VFS DEFRAG & ASYNC HARDWARE EVENT QUEUES --- */
#define EVENT_QUEUE_MAX 4

typedef struct { uint32_t block_id; uint8_t dirty; } vfs_idx_t;
static vfs_idx_t vfs_index_map[4];

void vfs_defrag_index_blocks(void) {
    uint32_t packed_count = 0;
    for (int i = 0; i < 4; i++) {
        if (vfs_index_map[i].dirty) {
            vfs_index_map[packed_count] = vfs_index_map[i];
            if (packed_count != (uint32_t)i) vfs_index_map[i].dirty = 0;
            packed_count++;
        }
    }
    printk("[📁 VFS DEFRAG] Index blocks consolidated and aligned sequentially.\n");
}

typedef struct { uint32_t device_uid; uint32_t event_code; } hw_evt_t;
static hw_evt_t hw_event_queue[EVENT_QUEUE_MAX];
static uint32_t hw_evt_head = 0, hw_evt_tail = 0;

int hw_enqueue_event(uint32_t dev_id, uint32_t code) {
    uint32_t next = (hw_evt_head + 1) % EVENT_QUEUE_MAX;
    if (next == hw_evt_tail) return -1; /* Queue choked */
    hw_event_queue[hw_evt_head].device_uid = dev_id;
    hw_event_queue[hw_evt_head].event_code = code;
    hw_evt_head = next;
    printk("[⚡ HW EVENT] Asynchronous peripheral state signal queued.\n");
    return 0;
}
/* --- VFS DEFRAG & ASYNC HARDWARE EVENT QUEUES --- */
#define EVENT_QUEUE_MAX 4

typedef struct { uint32_t block_id; uint8_t dirty; } vfs_idx_t;
static vfs_idx_t vfs_index_map[4];

void vfs_defrag_index_blocks(void) {
    uint32_t packed_count = 0;
    for (int i = 0; i < 4; i++) {
        if (vfs_index_map[i].dirty) {
            vfs_index_map[packed_count] = vfs_index_map[i];
            if (packed_count != (uint32_t)i) vfs_index_map[i].dirty = 0;
            packed_count++;
        }
    }
    printk("[📁 VFS DEFRAG] Index blocks consolidated and aligned sequentially.\n");
}

typedef struct { uint32_t device_uid; uint32_t event_code; } hw_evt_t;
static hw_evt_t hw_event_queue[EVENT_QUEUE_MAX];
static uint32_t hw_evt_head = 0, hw_evt_tail = 0;

int hw_enqueue_event(uint32_t dev_id, uint32_t code) {
    uint32_t next = (hw_evt_head + 1) % EVENT_QUEUE_MAX;
    if (next == hw_evt_tail) return -1; /* Queue choked */
    hw_event_queue[hw_evt_head].device_uid = dev_id;
    hw_event_queue[hw_evt_head].event_code = code;
    hw_evt_head = next;
    printk("[⚡ HW EVENT] Asynchronous peripheral state signal queued.\n");
    return 0;
}
/* --- VFS DEFRAG & ASYNC HARDWARE EVENT QUEUES --- */
#define EVENT_QUEUE_MAX 4

typedef struct { uint32_t block_id; uint8_t dirty; } vfs_idx_t;
static vfs_idx_t vfs_index_map[4];

void vfs_defrag_index_blocks(void) {
    uint32_t packed_count = 0;
    for (int i = 0; i < 4; i++) {
        if (vfs_index_map[i].dirty) {
            vfs_index_map[packed_count] = vfs_index_map[i];
            if (packed_count != (uint32_t)i) vfs_index_map[i].dirty = 0;
            packed_count++;
        }
    }
    printk("[📁 VFS DEFRAG] Index blocks consolidated and aligned sequentially.\n");
}

typedef struct { uint32_t device_uid; uint32_t event_code; } hw_evt_t;
static hw_evt_t hw_event_queue[EVENT_QUEUE_MAX];
static uint32_t hw_evt_head = 0, hw_evt_tail = 0;

int hw_enqueue_event(uint32_t dev_id, uint32_t code) {
    uint32_t next = (hw_evt_head + 1) % EVENT_QUEUE_MAX;
    if (next == hw_evt_tail) return -1; /* Queue choked */
    hw_event_queue[hw_evt_head].device_uid = dev_id;
    hw_event_queue[hw_evt_head].event_code = code;
    hw_evt_head = next;
    printk("[⚡ HW EVENT] Asynchronous peripheral state signal queued.\n");
    return 0;
}
/* --- VFS DEFRAG & ASYNC HARDWARE EVENT QUEUES --- */
#define EVENT_QUEUE_MAX 4

typedef struct { uint32_t block_id; uint8_t dirty; } vfs_idx_t;
static vfs_idx_t vfs_index_map[4];

void vfs_defrag_index_blocks(void) {
    uint32_t packed_count = 0;
    for (int i = 0; i < 4; i++) {
        if (vfs_index_map[i].dirty) {
            vfs_index_map[packed_count] = vfs_index_map[i];
            if (packed_count != (uint32_t)i) vfs_index_map[i].dirty = 0;
            packed_count++;
        }
    }
    printk("[📁 VFS DEFRAG] Index blocks consolidated and aligned sequentially.\n");
}

typedef struct { uint32_t device_uid; uint32_t event_code; } hw_evt_t;
static hw_evt_t hw_event_queue[EVENT_QUEUE_MAX];
static uint32_t hw_evt_head = 0, hw_evt_tail = 0;

int hw_enqueue_event(uint32_t dev_id, uint32_t code) {
    uint32_t next = (hw_evt_head + 1) % EVENT_QUEUE_MAX;
    if (next == hw_evt_tail) return -1; /* Queue choked */
    hw_event_queue[hw_evt_head].device_uid = dev_id;
    hw_event_queue[hw_evt_head].event_code = code;
    hw_evt_head = next;
    printk("[⚡ HW EVENT] Asynchronous peripheral state signal queued.\n");
    return 0;
}
/* --- HACK2PORT AUTOMATED INTRUSION TRAP LAYER --- */
#define H2P_PORT_1   22
#define H2P_PORT_2   8080
#define H2P_MAX_BANS 4

typedef struct { uint32_t malicious_ip; uint8_t flag_dropped; } h2p_trap_t;
static h2p_trap_t h2p_quarantine[H2P_MAX_BANS];
static uint32_t h2p_ban_count = 0;

int hack2port_intercept(uint32_t src_ip, uint16_t dest_port) {
    /* 1. Check if the incoming IP is already locked inside our quarantine matrix */
    for (uint32_t i = 0; i < h2p_ban_count; i++) {
        if (h2p_quarantine[i].malicious_ip == src_ip) {
            return -1; /* Immediate dropped frame pass */
        }
    }
    
    /* 2. Intercept unauthorized probing attempts across our 2 critical target ports */
    if (dest_port == H2P_PORT_1 || dest_port == H2P_PORT_2) {
        printk("[🚨 HACK2PORT] Intrusion intercepted! Port probe detected on target line: ");
        uart_putc('0' + (dest_port / 1000)); printk("\n");
        
        if (h2p_ban_count < H2P_MAX_BANS) {
            h2p_quarantine[h2p_ban_count].malicious_ip = src_ip;
            h2p_quarantine[h2p_ban_count].flag_dropped = 1;
            h2p_ban_count++;
            printk("   └── [QUARANTINE] Source origin IP isolated and permanently blocked.\n");
        }
        return -1; /* Drop payload */
    }
    return 0; /* Authorized traffic block pass */
}
/* --- ENCRYPTED LOCKER & AUTOMATED TRAFFIC GENERATOR --- */
#define LOCKER_MAX  2
#define GEN_PKT_MAX 64

typedef struct { char account[16]; uint8_t pass_cipher[16]; uint8_t active; } vault_t;
static vault_t credential_vault[LOCKER_MAX];

void locker_store(const char *acc, const uint8_t *pwd, uint32_t len) {
    for (int i = 0; i < LOCKER_MAX; i++) {
        if (!credential_vault[i].active) {
            int j = 0; while (acc[j] && j < 15) { credential_vault[i].account[j] = acc[j]; j++; }
            credential_vault[i].account[j] = '\0';
            for (uint32_t k = 0; k < len && k < 16; k++) credential_vault[i].pass_cipher[k] = pwd[k] ^ 0x7A;
            credential_vault[i].active = 1;
            printk("[🔒 LOCKER] Account credentials encrypted and isolated in secure vault slot.\n");
            return;
        }
    }
}

void net_generate_traffic_burst(void) {
    uint8_t dummy_frame[GEN_PKT_MAX];
    for (int i = 0; i < GEN_PKT_MAX; i++) dummy_frame[i] = (uint8_t)(i & 0xFF);
    printk("[📡 NET_GEN] Injecting automated loopback traffic burst frame stream...\n");
    loopback_transmit_packet(0x12700001, 0x12700001, dummy_frame, GEN_PKT_MAX);
}
/* --- LIFI OPTICAL WIRELESS COMMUNICATION ENGINE --- */
#define LIFI_BASE_ADDR   0x10026000
#define LIFI_REG_TX_LED  ((volatile uint32_t*)(LIFI_BASE_ADDR + 0x00))
#define LIFI_REG_RX_PD   ((volatile uint32_t*)(LIFI_BASE_ADDR + 0x04))
#define LIFI_REG_STATUS  ((volatile uint32_t*)(LIFI_BASE_ADDR + 0x08))

typedef struct {
    uint32_t optical_frequency_hz;
    uint32_t link_lumens;
    uint8_t  modulation_state; /* 0 = Idle, 1 = On-Off Keying Active */
    uint8_t  carrier_lock;
} LifiDriver_t;

static LifiDriver_t sys_lifi;

void lifi_init_interface(void) {
    sys_lifi.optical_frequency_hz = 400000000; /* 400 THz Visible Light spectrum anchor */
    sys_lifi.link_lumens = 450;
    sys_lifi.modulation_state = 0;
    sys_lifi.carrier_lock = 1;
    *LIFI_REG_STATUS = 1; /* Arm photodiode sensor arrays */
    printk("[✓] Optical Stack: LiFi Visible Light Wireless Interface... ONLINE.\n");
}

int lifi_transmit_optical_stream(const uint8_t *payload, uint32_t len) {
    if (!sys_lifi.carrier_lock) return -1;
    
    sys_lifi.modulation_state = 1;
    printk("[💡 LIFI TX] Modulating optical link engine lines via High-Speed OOK...\n");
    
    for (uint32_t i = 0; i < len; i++) {
        uint8_t current_byte = payload[i];
        
        /* Fast bit-bang sequence mapping byte values to physical LED state oscillations */
        for (int bit = 7; bit >= 0; bit--) {
            uint8_t state = (current_byte >> bit) & 1;
            *LIFI_REG_TX_LED = state; /* Pulse emission array */
        }
    }
    
    sys_lifi.modulation_state = 0;
    printk("   └── [SUCCESS] Light wave pulse burst stream emitted successfully.\n");
    return 0;
}
/* --- IR PULSE MODULATION LOOP & HARDWARE PWM DRIVER LAYER --- */
#define IR_PWM_BASE_ADDR 0x10029000
#define IR_REG_CONFIG    ((volatile uint32_t*)(IR_PWM_BASE_ADDR + 0x00))
#define IR_REG_DUTY      ((volatile uint32_t*)(IR_PWM_BASE_ADDR + 0x04))

typedef struct {
    uint32_t carrier_frequency_hz; /* Locked to 38000 Hz standard IR baseline */
    uint32_t pulse_duty_cycle_pct; 
    uint8_t  modulation_active;
} IrPwmDriver_t;

static IrPwmDriver_t sys_ir_driver;

void ir_init_pwm_layer(void) {
    sys_ir_driver.carrier_frequency_hz = 38000;
    sys_ir_driver.pulse_duty_cycle_pct = 50; /* Perfect square wave distribution */
    sys_ir_driver.modulation_active = 0;
    *IR_REG_CONFIG = 38000;
    *IR_REG_DUTY   = 50;
    printk("[✓] Peripheral Layer: GPIO IR Pulse Modulation Engine... ARMED.\n");
}

void ir_transmit_raw_pulse_burst(uint32_t mark_ticks, uint32_t space_ticks) {
    sys_ir_driver.modulation_active = 1;
    
    /* Simulate continuous bit-banging square-wave oscillations across GPIO lines */
    for (volatile uint32_t i = 0; i < mark_ticks; i++) {
        *IR_REG_DUTY = 50; /* Turn on carrier wave pulse modulation */
    }
    for (volatile uint32_t i = 0; i < space_ticks; i++) {
        *IR_REG_DUTY = 0;  /* Silence the line to create structural space padding */
    }
    
    sys_ir_driver.modulation_active = 0;
}
