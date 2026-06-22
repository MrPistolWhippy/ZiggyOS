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
