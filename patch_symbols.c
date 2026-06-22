void print(const char *s) { while(*s) { *(volatile char *)0x10000000 = *s++; } }
void printf(const char *fmt, ...) { print(fmt); }
void _start(void) { main(); while(1); }
