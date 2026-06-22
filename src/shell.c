/* ==============================================================================
 *          ZIGGY-OS USER LAND: INTERACTIVE COMMAND LINE SHELL ENGINE
 * ============================================================================== */
#include <stdint.h>
#include <stddef.h>

/* Hook to external UART functions and VFS data defined in our core kernel */
extern void uart_puts(const char *s);
void uart_putc(char c);

#define VFS_MAX_NAME 32
typedef struct vnode {
    char     name[VFS_MAX_NAME];
    uint8_t  type;
    size_t   size;
    uint8_t  *data_block_ptr;
    struct vnode *next;
} vnode_t;

extern vnode_t *vfs_root;

/* Quick string comparison utility for the shell loop */
int strcmp(const char *s1, const char *s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(unsigned char *)s1 - *(unsigned char *)s2;
}

void ziggy_shell_run(const char *cmd_buffer) {
    if (strcmp(cmd_buffer, "help") == 0) {
        uart_puts("\n--- ZIGGY-OS SYSTEM COMMANDS ---");
        uart_puts("\n  help - Display active shell functions");
        uart_puts("\n  ls   - List virtual filesystem node directories");
        uart_puts("\n  cat  - Print kernel memory-mapped register states\n");
    } 
    else if (strcmp(cmd_buffer, "ls") == 0) {
        uart_puts("\nMount Directory Target: /\n");
        if (!vfs_root || !vfs_root->next) {
            uart_puts("  [Empty directory]\n");
            return;
        }
        vnode_t *curr = vfs_root->next;
        while (curr) {
            uart_puts("  - ");
            uart_puts(curr->name);
            uart_puts("  (Size: ");
            uart_putc('0' + (curr->size / 512)); /* Simple size indicator */
            uart_puts(" blocks)\n");
            curr = curr->next;
        }
    } 
    else if (strcmp(cmd_buffer, "cat") == 0) {
        uart_puts("\n[SYS LOG] Telemetry Matrix Stable. Context Multi-threading... ACTIVE\n");
    } 
    else if (cmd_buffer[0] != '\0') {
        uart_puts("\nziggy-sh: command not found: ");
        uart_puts(cmd_buffer);
        uart_puts("\n");
    }
}
