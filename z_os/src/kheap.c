#include <stdint.h>
#include <stddef.h>

extern uint8_t _kernel_end;
static uint32_t heap_top = (uint32_t)&_kernel_end;

// Metadata header prepended to every memory block allocation
typedef struct BlockHeader {
    size_t size;               // Size of the data region following this header
    int is_free;               // 1 if recycled/free, 0 if active and in use
    struct BlockHeader* next;  // Pointer to the next chunk in our memory pool
} BlockHeader_t;

static BlockHeader_t* block_list_head = NULL;

void* kmalloc(size_t size) {
    // Enforce 4-Byte CPU boundary alignment
    size = (size + 3) & ~3;
    
    BlockHeader_t* current = block_list_head;
    BlockHeader_t* best_fit = NULL;

    // First Fit / Best Fit Scan: Look for an existing, recycled block that fits
    while (current != NULL) {
        if (current->is_free && current->size >= size) {
            if (best_fit == NULL || current->size < best_fit->size) {
                best_fit = current;
            }
        }
        current = current->next;
    }

    // If an available recycled block is found, claim it
    if (best_fit != NULL) {
        best_fit->is_free = 0;
        return (void*)((uint32_t)best_fit + sizeof(BlockHeader_t));
    }

    // Otherwise, expand the heap boundary forward (Bump fallback)
    uint32_t raw_address = heap_top;
    heap_top += (size + sizeof(BlockHeader_t));

    BlockHeader_t* new_block = (BlockHeader_t*)raw_address;
    new_block->size = size;
    new_block->is_free = 0;
    new_block->next = NULL;

    // Append the newly carved block to our global linked tracker list
    if (block_list_head == NULL) {
        block_list_head = new_block;
    } else {
        current = block_list_head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_block;
    }

    // Return the data pointer directly following the metadata header
    return (void*)(raw_address + sizeof(BlockHeader_t));
}

void kfree(void* ptr) {
    if (ptr == NULL) return;

    // Roll back the pointer address to expose the hidden block header metadata
    BlockHeader_t* header = (BlockHeader_t*)((uint32_t)ptr - sizeof(BlockHeader_t));
    header->is_free = 1; // Release the block back to the pool
}
