typedef unsigned int size_t;
volatile unsigned short* const vga = (volatile unsigned short*)0xB8000;
unsigned int cursor_x = 0; unsigned int cursor_y = 2;

void local_print(const char* msg) {
    for (int i = 0; msg[i] != 0; i++) {
        if (msg[i] == '\n') { cursor_x = 0; cursor_y++; }
        else { vga[cursor_y * 80 + cursor_x] = (0x1F00) | msg[i]; cursor_x++; }
        if (cursor_x >= 80) { cursor_x = 0; cursor_y++; }
    }
}
void outb(unsigned short port, unsigned char val) { __asm__ volatile("outb %0, %1" : : "a"(val), "Nd"(port)); }
unsigned char inb(unsigned short port) { unsigned char ret; __asm__ volatile("inb %1, %0" : "=a"(ret) : "Nd"(port)); return ret; }

typedef struct {
    char name[16];
    unsigned int size;
    unsigned int data_ptr;
} VfsFile;

VfsFile file_table[8];
unsigned int file_count = 0;
unsigned int heap_bump_ptr = 0x00500000;

unsigned int local_malloc(unsigned int size) {
    unsigned int alloc = heap_bump_ptr;
    heap_bump_ptr += (size + 3) & ~3;
    return alloc;
}

// --- Subsystem A: Text-Matching File Reader Engine (cat) ---
int local_strcmp(const char* s1, const char* s2) {
    int i = 0;
    while (s1[i] != 0 && s2[i] != 0) {
        if (s1[i] != s2[i]) return 0;
        i++;
    }
    return (s1[i] == s2[i]);
}

void cmd_cat(const char* filename) {
    local_print("\n[VFS CAT] Reading file buffer metrics:\n");
    for (unsigned int i = 0; i < file_count; i++) {
        if (local_strcmp(file_table[i].name, filename)) {
            if (file_table[i].size == 0) {
                local_print(" -> File content is completely empty.\n");
                return;
            }
            local_print(" -> Contents: ");
            local_print((const char*)file_table[i].data_ptr);
            local_print("\n");
            return;
        }
    }
    local_print("[VFS ERROR] Specified file node target not found in allocation index.\n");
}

void vfs_init(void) {
    char* name_target = file_table[0].name;
    name_target[0]='m'; name_target[1]='o'; name_target[2]='t'; name_target[3]='d'; name_target[4]=0;
    file_table[0].size = 18;
    file_table[0].data_ptr = local_malloc(32);
    char* content = (char*)file_table[0].data_ptr;
    const char* src = "We love you heaps!";
    int i = 0; while(src[i] != 0) { content[i] = src[i]; i++; } content[i] = 0;
    file_count = 1;
    local_print("[VFS] Mounted navigation tracking node with motd parameters.\n");
}

// --- Subsystem B: Hardware Matrix Hex Inversion Module ---
void cmd_invert(unsigned int value) {
    local_print("\n[HEX INVERTER] Initializing bitwise hardware lookup transformation:\n");
    local_print(" -> Original Register Input:  0x");
    for (int i = 7; i >= 0; i--) {
        vga[cursor_y * 80 + cursor_x] = 0x1E00 | ("0123456789ABCDEF"[(value >> (i * 4)) & 0x0F]); cursor_x++;
    }
    
    // Execute a bitwise NOT hardware inversion on the input register metrics
    unsigned int inverted = ~value;
    
    local_print("\n -> Inverted Matrix Result:   0x");
    for (int i = 7; i >= 0; i--) {
        vga[cursor_y * 80 + cursor_x] = 0x1A00 | ("0123456789ABCDEF"[(inverted >> (i * 4)) & 0x0F]); cursor_x++;
    }
    local_print("\n");
}
void keyboard_handler(void) { local_print("\nZiggyOS:# [Key Interrupt Handled]\n"); }
void pit_scheduler_handler(void) { outb(0x20, 0x20); }
void rtl8139_interrupt_handler(void) { outb(0xA0, 0x20); outb(0x20, 0x20); }

void kmain_32(void) {
    local_print("ZiggyOS Native Local Dual-Feature Core Ready!\n");
    vfs_init();
    cmd_cat("motd");     // Test text file reading mechanics
    cmd_invert(0xF0F00F0F); // Validate bitwise hardware lookup tracking transforms
    local_print("\nZiggyOS:# ");
    while(1) { __asm__ volatile("hlt"); }
}
unsigned char* memcpy(unsigned char* dest, const unsigned char* src, size_t n) {
    for(size_t i = 0; i < n; i++) dest[i] = src[i]; return dest;
}
unsigned char* memset(unsigned char* dest, unsigned char c, size_t n) {
    for(size_t i = 0; i < n; i++) dest[i] = c; return dest;
}
void __zig_probe_stack(void) {}
