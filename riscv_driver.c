#include <stdio.h>
#include <stdint.h>
extern int verify_mesh_block(uint64_t *addr, uint64_t tok, uint64_t len);
int main() {
    uint64_t led[4] = {0x8F4625B0, 0x2C26B46B, 0x4A123F85, 0};
    uint64_t tok = 0x8F4625B0 ^ 0x2C26B46B ^ 0x4A123F85;
    printf(verify_mesh_block(led, tok, 4) ? "[PASS] MATCH\n" : "[FAIL] DRIFT\n");
    return 0;
}
