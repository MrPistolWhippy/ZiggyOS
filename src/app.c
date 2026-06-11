#include "io.h"

// 1. Retro Matrix Screensaver System
unsigned int rand_seed = 12345;
unsigned int quick_rand() {
    rand_seed = (rand_seed * 1103515245 + 12345) & 0x7FFFFFFF;
    return rand_seed;
}

void app_matrix_screensaver() {
    volatile unsigned short* vram = (volatile unsigned short*)0xB8000;
    // Wipe screen to blank cells
    for(int i = 0; i < 2000; i++) vram[i] = (0x0F << 8) | ' ';
    
    print("Press any key to exit Matrix mode...\n");
    
    // Core stream execution loop
    while(!keyboard_has_key()) {
        int x = quick_rand() % 80;
        int y = quick_rand() % 25;
        char c = 33 + (quick_rand() % 93); // Generate random printable ASCII character
        
        int offset = y * 80 + x;
        vram[offset] = (0x0A << 8) | c; // Emerald Green text character attribute style
        
        // Fast execution delay loop
        for(volatile int d = 0; d < 10000; d++);
    }
    shell_clear();
}

// 2. Simple Command Buffer Math Calculator
void app_run_calculator(const char* expr) {
    int num1 = 0, num2 = 0;
    char op = 0;
    int idx = 0;

    // Fast inline string index parsing loop
    while(expr[idx] == ' ') idx++; // Skip command space buffers
    
    while(expr[idx] >= '0' && expr[idx] <= '9') {
        num1 = num1 * 10 + (expr[idx] - '0');
        idx++;
    }
    while(expr[idx] == ' ') idx++;
    if(expr[idx] == '+' || expr[idx] == '-' || expr[idx] == '*' || expr[idx] == '/') {
        op = expr[idx];
        idx++;
    }
    while(expr[idx] == ' ') idx++;
    while(expr[idx] >= '0' && expr[idx] <= '9') {
        num2 = num2 * 10 + (expr[idx] - '0');
        idx++;
    }

    print("\nResult: ");
    int res = 0;
    if(op == '+') res = num1 + num2;
    else if(op == '-') res = num1 - num2;
    else if(op == '*') res = num1 * num2;
    else if(op == '/') {
        if(num2 == 0) { print("Error: Div by 0!\n"); return; }
        res = num1 / num2;
    } else {
        print("Invalid math string format!\n");
        return;
    }

    // Format output integer tracking array back to characters
    char buf[16];
    int i = 0;
    if(res == 0) { print_char('0'); print("\n"); return; }
    if(res < 0) { print_char('-'); res = -res; }
    while(res > 0) { buf[i++] = '0' + (res % 10); res /= 10; }
    while(i > 0) print_char(buf[--i]);
    print("\n");
}

// 3. Hardware Asset Architecture Inspector
void app_print_system_info() {
    print("\n========================================\n");
    print("       ZIGGYOS HARDWARE INSPECTOR       \n");
    print("========================================\n");
    print(" CPU Architecture: x86 Protected Mode (32-Bit)\n");
    print(" Core Base Mode:   Freestanding Microkernel\n");
    print(" Video Memory:     VGA Character Map (0xB8000)\n");
    print(" Timer Subsystem:  8253 Programmable Interval Timer\n");
    print(" Sound Device:     PC System Logic Port 0x61\n");
    print(" Storage Engine:   Dynamic Volatile RAMFS Drive\n");
    print("========================================\n");
}
