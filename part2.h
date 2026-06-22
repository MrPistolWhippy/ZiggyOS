#include "part1.h"
#define MAX_INTERFACES 3
#define IF_QUEUE_LIMIT 4
typedef struct { uint32_t packet_id; uint32_t weight; } pkt_t;
typedef struct { pkt_t queue[IF_QUEUE_LIMIT]; int head; int tail; uint32_t tokens; uint32_t token_rate; } interface_t;
static interface_t interfaces[MAX_INTERFACES];
static int rr_index = 0;
static void scheduler_init() { for(int i=0; i<MAX_INTERFACES; i++) { interfaces[i].head=0; interfaces[i].tail=0; interfaces[i].tokens=20; interfaces[i].token_rate=(i+1)*5; } }
static int scheduler_enqueue(int idx, uint32_t pid, uint32_t w) { interface_t *iface=&interfaces[idx]; if(((iface->tail+1)%IF_QUEUE_LIMIT)==iface->head) return ZIGGY_ERR; iface->queue[iface->tail].packet_id=pid; iface->queue[iface->tail].weight=w; iface->tail=(iface->tail+1)%IF_QUEUE_LIMIT; return ZIGGY_OK; }
static void scheduler_process_tx() { printf("[TX] Multi-Interface Round-Robin Matrix:\n"); for(int i=0; i<MAX_INTERFACES; i++) { int idx=(rr_index+i)%MAX_INTERFACES; interface_t *iface=&interfaces[idx]; iface->tokens+=iface->token_rate; while(iface->head!=iface->tail) { pkt_t pkt=iface->queue[iface->head]; if(iface->tokens>=pkt.weight) { iface->tokens-=pkt.weight; printf("  -> IF %d dispatched Packet ID: %d (Cost: %d)\n", idx, pkt.packet_id, pkt.weight); iface->head=(iface->head+1)%IF_QUEUE_LIMIT; } else { printf("  -> IF %d Traffic Shaped! Awaiting tokens\n", idx); break; } } } rr_index=(rr_index+1)%MAX_INTERFACES; }
typedef struct { volatile uint32_t lock; uint32_t owner_priority; } spinlock_t;
static spinlock_t global_mutex={0,0};
static volatile uint32_t shared_resource=0;
static uint32_t atomic_amoswap(volatile uint32_t *addr, uint32_t val) { return __sync_lock_test_and_set(addr, val); }
static void ziggy_mutex_lock(spinlock_t *l, uint32_t p) { while(atomic_amoswap(&l->lock, 1)==1) { if(p>l->owner_priority && l->owner_priority!=0) l->owner_priority=p; usleep(1); } l->owner_priority=p; }
static void ziggy_mutex_unlock(spinlock_t *l) { l->owner_priority=0; __sync_lock_release(&l->lock); }
