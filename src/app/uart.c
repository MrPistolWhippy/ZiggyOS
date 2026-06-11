#define U ((volatile int*)0x40004404)
void putc(char c){ *U=c; }