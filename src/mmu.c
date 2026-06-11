#include <stdint.h>

// Align memory structures to standard 4KB page boundaries
__attribute__((aligned(4096))) uint32_t page_directory[1024];
__attribute__((aligned(4096))) uint32_t first_page_table[1024];

extern void print(const char* str);

void init_mmu(void) {
    print("MMU: Initializing 32-bit x86 virtual page directory matrix...\n");

    // 1. Clear the Directory entries (Set to Supervisor, Read/Write, Not Present)
    for (int i = 0; i < 1024; i++) {
        page_directory[i] = 0x00000002;
    }

    // 2. Identity map the first 4 Megabytes of physical memory RAM
    for (unsigned int i = 0; i < 1024; i++) {
        // Attributes: Supervisor level, Read/Write, Present (0x03)
        first_page_table[i] = (i * 4096) | 3;
    }

    // 3. Bind the first Page Table into the Page Directory index 0
    page_directory[0] = ((unsigned int)first_page_table) | 3;

    // 4. Load Page Directory pointer into control register CR3 and flip CR0 to enable Paging
    asm volatile(
        "mov %0, %%cr3\n\t"
        "mov %%cr0, %%eax\n\t"
        "or $0x80000000, %%eax\n\t"
        "mov %%eax, %%cr0\n\t"
        :
        : "r"(page_directory)
        : "eax", "memory"
    );

    print("MMU: Identity paging successfully locked into CR0/CR3 hardware registers!\n");
}
