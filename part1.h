#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#define ZIGGY_OK 0
#define ZIGGY_ERR -1
#define DH_LIMBS 4 
typedef struct { uint32_t limbs[DH_LIMBS]; } uint128_t;
void dh_print(const char* l, uint128_t a) { printf("%s: 0x%08x%08x%08x%08x\n", l, a.limbs[0], a.limbs[1], a.limbs[2], a.limbs[3]); }
int dh_is_zero(uint128_t a) { for(int i=0; i<DH_LIMBS; i++) if(a.limbs[i]!=0) return 0; return 1; }
int dh_compare(uint128_t a, uint128_t b) { for(int i=DH_LIMBS-1; i>=0; i--) { if(a.limbs[i]>b.limbs[i]) return 1; if(a.limbs[i]<b.limbs[i]) return -1; } return 0; }
uint128_t dh_shift_right(uint128_t a) { uint128_t res={{0}}; uint32_t c=0; for(int i=DH_LIMBS-1; i>=0; i--) { uint32_t nc=(a.limbs[i]&1)<<31; res.limbs[i]=(a.limbs[i]>>1)|c; c=nc; } return res; }
uint128_t dh_shift_left(uint128_t a) { uint128_t res={{0}}; uint32_t c=0; for(int i=0; i<DH_LIMBS; i++) { uint32_t nc=(a.limbs[i]&0x80000000)?1:0; res.limbs[i]=(a.limbs[i]<<1)|c; c=nc; } return res; }
uint128_t dh_add(uint128_t a, uint128_t b) { uint128_t res; uint64_t c=0; for(int i=0; i<DH_LIMBS; i++) { uint64_t s=(uint64_t)a.limbs[i]+b.limbs[i]+c; res.limbs[i]=(uint32_t)s; c=s>>32; } return res; }
uint128_t dh_sub(uint128_t a, uint128_t b) { uint128_t res; int64_t br=0; for(int i=0; i<DH_LIMBS; i++) { int64_t d=(int64_t)a.limbs[i]-b.limbs[i]-br; if(d<0) { res.limbs[i]=(uint32_t)(d+0x100000000ULL); br=1; } else { res.limbs[i]=(uint32_t)d; br=0; } } return res; }
uint128_t dh_mod(uint128_t a, uint128_t m) { uint128_t t=m, r=a; int s=0; if(dh_is_zero(m)) return a; while(dh_compare(r,t)>=0 && !(t.limbs[DH_LIMBS-1]&0x80000000)) { uint128_t ns=dh_shift_left(t); if(dh_compare(r,ns)<0) break; t=ns; s++; } for(int i=0; i<=s; i++) { if(dh_compare(r,t)>=0) r=dh_sub(r,t); t=dh_shift_right(t); } return r; }
uint128_t dh_mul_mod(uint128_t a, uint128_t b, uint128_t m) { uint128_t res={{0}}, t=dh_mod(a,m); for(int i=0; i<DH_LIMBS*32; i++) { if(b.limbs[i/32]&(1U<<(i%32))) { res=dh_add(res,t); res=dh_mod(res,m); } t=dh_add(t,t); t=dh_mod(t,m); } return res; }
uint128_t dh_mod_exp(uint128_t b, uint128_t e, uint128_t m) { uint128_t res={{1,0,0,0}}, base=dh_mod(b,m), ex=e; while(!dh_is_zero(ex)) { if(ex.limbs[0]&1) res=dh_mul_mod(res,base,m); base=dh_mul_mod(base,base,m); ex=dh_shift_right(ex); } return res; }
#define BUDDY_MIN_ORDER 4
#define BUDDY_MAX_ORDER 8
#define MEM_POOL_SIZE 512
static uint8_t kmem_pool[MEM_POOL_SIZE] __attribute__((aligned(16)));
typedef struct { uint8_t free_buckets[BUDDY_MAX_ORDER + 1]; uint32_t allocation_offsets[BUDDY_MAX_ORDER + 1]; } buddy_t;
static buddy_t allocator;
static void buddy_init() { memset(kmem_pool,0,MEM_POOL_SIZE); memset(&allocator,0,sizeof(allocator)); allocator.free_buckets[BUDDY_MAX_ORDER]=2; }
static void* ziggy_sys_allocate(uint32_t b) { int o=BUDDY_MIN_ORDER; o=4; while((1U<<o)<b && o<=BUDDY_MAX_ORDER) o++; if(o>BUDDY_MAX_ORDER) return NULL; for(int i=o; i<=BUDDY_MAX_ORDER; i++) { if(allocator.free_buckets[i]>0) { allocator.free_buckets[i]--; uint32_t co=allocator.allocation_offsets[i]; allocator.allocation_offsets[i]+=(1U<<i); for(int j=i; j>o; j--) { allocator.free_buckets[j-1]+=2; allocator.allocation_offsets[j-1]=co; } return (void*)&kmem_pool[co]; } } return NULL; }
static void ziggy_ecall_vector(uint32_t s, uint32_t a, void** o) { if(s==0x10) *o=ziggy_sys_allocate(a); else *o=NULL; }
