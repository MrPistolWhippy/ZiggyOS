#include <stdint.h>

extern void print(const char *str);

// Hardware Flash Controller Register Base (Standard x86 Memory Map)
#define FLASH_R_BASE   0x40022000
#define FLASH_CR       ((volatile uint32_t *)(FLASH_R_BASE + 0x10)) // Control Register
#define FLASH_SR       ((volatile uint32_t *)(FLASH_R_BASE + 0x0C)) // Status Register
#define FLASH_AR       ((volatile uint32_t *)(FLASH_R_BASE + 0x14)) // Address Register

#define FLASH_CR_PG    (1 << 0)   // Flash Programming activation bit flag
#define FLASH_SR_BSY   (1 << 16)  // Flash Busy status verification bit flag

// Low-level hardware block driver utility to persistently commit half-words
void flash_write_sector_block(uint32_t address, uint16_t data) {
    // 1. Wait defensively until any pre-existing flash operation clears out
    while (*FLASH_SR & FLASH_SR_BSY);
    
    // 2. Set the Programming enable bit flag in the control register
    *FLASH_CR |= FLASH_CR_PG;
    
    // 3. Register the exact physical target sector memory offset address
    *FLASH_AR = address;
    
    // 4. Fire the physical commit execution write to the flash address sector
    *((volatile uint16_t *)address) = data;
    
    // 5. Wait defensively for the controller hardware flag to report successful execution clear
    while (*FLASH_SR & FLASH_SR_BSY);
    
    // 6. Strip out the flash programming permission bit flag to re-secure the block line
    *FLASH_CR &= ~FLASH_CR_PG;
}

void init_flash_persistence_driver(void) {
    print("[DRIVER] Persistent Non-Volatile Flash Memory Block Controller initialized.\n");
}
