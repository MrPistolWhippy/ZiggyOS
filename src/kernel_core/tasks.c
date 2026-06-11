#include <stdint.h>
typedef struct { uint32_t *sp; uint32_t id; uint32_t state; } TCB_t;
TCB_t tasks[4]; uint32_t current_id = 0;
void yield() { uint32_t next = (current_id + 1) % 4; current_id = next; }
