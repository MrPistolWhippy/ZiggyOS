#include <stdint.h>
#define NVIC_ISER0 ((volatile uint32_t *)0xE000E100)
void enable_irq(uint8_t irq_num) { *NVIC_ISER0 |= (1 << (irq_num & 0x1F)); }
void exti_handler() { volatile uint32_t *pr = (uint32_t *)0x40013C14; *pr |= 0x1; }
