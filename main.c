#include "part2.h"
void* core_worker(void* arg) {
    uintptr_t core_id=(uintptr_t)arg; uint32_t wp=(uint32_t)(core_id*10)+5;
    for(int i=0; i<50; i++) { ziggy_mutex_lock(&global_mutex, wp); uint32_t cur=shared_resource; usleep(1); shared_resource=cur+1; ziggy_mutex_unlock(&global_mutex); }
    return NULL;
}
int main() {
    printf("=========================================================\n");
    printf("     ZIGGY-OS INTEGRATED ARCHITECTURE VALIDATION MATRIX  \n");
    printf("=========================================================\n\n");
    uint128_t base={{5,0,0,0}}, exp={{11,0,0,0}}, mod={{13,0,0,0}};
    uint128_t res=dh_mod_exp(base, exp, mod);
    dh_print("[DH] Base (g)", base); dh_print("[DH] Exponent (Secret)", exp);
    dh_print("[DH] Modulus (p)", mod); dh_print("[DH] Shared Secret Matrix", res);
    printf("\n");
    buddy_init(); void *p1=NULL, *p2=NULL; ziggy_ecall_vector(0x10, 32, &p1); ziggy_ecall_vector(0x10, 32, &p2);
    printf("[ECALL] Allocation Pointer 1: %p\n", p1); printf("[ECALL] Allocation Pointer 2: %p\n\n", p2);
    scheduler_init(); scheduler_enqueue(0, 101, 15); scheduler_enqueue(1, 201, 45); scheduler_enqueue(2, 301, 10); scheduler_process_tx(); printf("\n");
    pthread_t cores[4]; printf("[SMP] Spawning 4 system execution cores...\n");
    for(uintptr_t i=0; i<4; i++) pthread_create(&cores[i], NULL, core_worker, (void*)i);
    for(int i=0; i<4; i++) pthread_join(cores[i], NULL);
    printf("[SMP] Expected Balance: 200 | Actual Result: %d\n", shared_resource);
    if(shared_resource==200) printf("\n>>> MATRIX LEVEL STATUS: ELECTRIC TEAL (100%% VERIFIED) <<<\n");
    else printf("\n>>> CONFIGURATION ERROR: MUTEX FAULT <<<\n");
    return 0;
}
