#include <stdint.h>

// Direct print implementation
void print(const char* str) { (void)str; }

// Missing kernel architecture and driver stubs
void watchdog_kick(void) {}
void init_syscall_vector_gate(void) {}
void init_fat_filesystem(void) {}
void init_ring1_device_drivers(void) {}
void jump_to_user_mode(void) {}
