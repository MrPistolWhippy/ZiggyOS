#include <stdint.h>

#define MAX_FILES 16
#define BLOCK_SIZE 512

void init_global_descriptor_table(void) {
    // Basic placeholder for GDT initialization
}

void enter_userland_sys(uint32_t user_stack, uint32_t user_entry) {
    // Explicit 32-bit ('l') instruction suffixes to pass validation on 64-bit cross-compilers
    asm volatile(
        "pushl $0x23\n"     /* User Data Segment Descriptor Selector */
        "pushl %0\n"        /* Target User Space Stack Pointer */
        "pushfl\n"          /* Push CPU Flags onto Stack Frame */
        "pushl $0x1B\n"     /* User Code Segment Descriptor Selector */
        "pushl %1\n"        /* Target User Space Execution Entry Point */
        "iretl\n"           /* Return from Interrupt to drop CPU privilege level */
        :
        : "r"(user_stack), "r"(user_entry)
        : "memory"
    );
}
