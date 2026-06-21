#ifndef ZIGGYOS_H
#define ZIGGYOS_H
#include <stdint.h>
#include <stddef.h>

void putc(char c);
void print(const char* str);
void* kmalloc(size_t size);

#endif
