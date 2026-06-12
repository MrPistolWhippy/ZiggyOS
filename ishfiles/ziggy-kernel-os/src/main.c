#include <stdint.h>
#include <string.h>

#define REG(addr)   (*(volatile uint32_t*)(addr))
#define UART_DR     REG(0x4000C000)
#define STK_CTRL    REG(0xE000E010)
#define STK_LOAD    REG(0xE000E014)

uint32_t t_stk[3][64];
uint32_t* t_sp[3];
uint32_t task_sleep[3] = {0, 0, 0};

void send(char c) {
    while ((REG(0x4000C004) & 2) == 0);
    REG(0x4000C000) = c;
}

void task1(void) { while(1) { send('1'); } }
void task2(void) { while(1) { send('2'); } }

void task3(void) {
    char b[32]; uint8_t idx = 0;
    while (1) {
        char c = UART_DR;
        if (c != 0) {
            send(c);
            if (c == 13 || c == 10) {
                b[idx] = 0;
                if (strcmp(b, "help") == 0) send('H');
                else if (strcmp(b, "status") == 0) send('S');
                idx = 0;
            } else if (idx < 31) {
                b[idx++] = c;
            }
        }
    }
}

void init_t(int id, void (*f)(void), uint32_t *stk) {
    stk[63] = 0x01000000;
    stk[62] = (uint32_t)f;
    t_sp[id] = &stk[46];
    task_sleep[id] = 0;
}

void Reset_Handler(void) {
    REG(0xE000ED94) = 0; REG(0x40087004) |= 0x30;
    REG(0x400FF428) = (1 << 4);
    
    init_t(0, task1, t_stk[0]);
    init_t(1, task2, t_stk[1]);
    init_t(2, task3, t_stk[2]);
    
    STK_LOAD = 16000 - 1;
    STK_CTRL = 0x07;
    __asm__ volatile ("cpsie i");
    task1();
}

void HardFault_Handler(void) { while(1); }
void PendSV_Handler(void) { while(1); }
void SysTick_Handler(void) { while(1); }
void UART0_Handler(void) { while(1); }

void app_diag_main(void) {
    // Diagnostic entry placeholder
    return;
}

/* System symbol patches */
void app_diag_main(void) {
    return;
}
void init_hardware_pit_timer(void) {}
int verify_mmu_lattice_token(void) { return 1; }
void run_automated_harness(void) {}
