#include <stdint.h>

// Core symbol overrides to satisfy tracking parameters
void _exit(int status) {
    (void)status;
    while (1);
}

void __stack_chk_fail(void) {
    while (1);
}
