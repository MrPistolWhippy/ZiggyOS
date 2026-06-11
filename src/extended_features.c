#include <stdint.h>

#define MAX_FILES 16
#define BLOCK_SIZE 512

void init_global_descriptor_table(void) {
    // Basic placeholder for GDT initialization
}

void enter_userland_sys(uint32_t user_stack, uint32_t user_entry) {
    asm volatile(
        "push $0x23\n"
        "push %0\n"
        "pushf\n"
        "push $0x1B\n"
        "push %1\n"
        "iret\n"
        :
        : "r"(user_stack), "r"(user_entry)
        : "memory"
    );
}
