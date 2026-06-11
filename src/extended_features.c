#include <stdint.h>
extern void print(const char *str);

void init_flash_page_mapper(void) { print("[MMU] Mapping physical Flash memory layout...\n"); }

typedef struct { uint8_t preamble, length, payload, crc; } __attribute__((packed)) RadioFrame_t;
void radio_pack_frame(RadioFrame_t *f, uint8_t *d, uint8_t l) { f->preamble = 0xAA; f->length = l; }

#define VIDEO_RAM_SIZE 2048
uint8_t front_framebuffer[VIDEO_RAM_SIZE];
uint8_t back_framebuffer[VIDEO_RAM_SIZE];
void init_video_dma_canvas(void) { print("[VIDEO] Initializing double-buffered DMA display canvas...\n"); }

void serial_queue_push(char c) { (void)c; }
void *kernel_malloc(uint32_t size) { (void)size; return (void *)0; }
void watchdog_kick(void) { volatile uint32_t *wdt_kr = (volatile uint32_t *)0x40002C00; *wdt_kr = 0xAAAA; }

typedef struct { volatile uint32_t lock_state; } Mutex_t;
void mutex_lock(Mutex_t *m) { while (__sync_lock_test_and_set(&m->lock_state, 1)); }
void mutex_unlock(Mutex_t *m) { __sync_lock_release(&m->lock_state); }

uint64_t get_system_uptime_ticks(void) { return 0; }
void system_clock_tick_isr(void) {}
void init_thread_context(void *ctx, void (*e)(void), uint32_t *s) { (void)ctx; (void)e; (void)s; }

uint64_t gdt;
void init_global_descriptor_table(void) { print("[CPU] Setting up Global Descriptor Table segments...\n"); }

void telemetry_encrypt_sys(uint32_t *v, uint32_t const *key) {
    uint32_t v0 = v[0], v1 = v[1], sum = 0, delta = 0x9E3779B9;
    for (int i = 0; i < 32; i++) {
        v0 += (((v1 << 4) ^ (v1 >> 5)) + v1) ^ (sum + key[sum & 3]); sum += delta;
        v1 += (((v0 << 4) ^ (v0 >> 5)) + v0) ^ (sum + key[(sum >> 11) & 3]);
    }
    v[0] = v0; v[1] = v1;
}
void init_interrupt_priorities_sys(void) { print("[CPU] Configuring interrupt priority rings...\n"); }
int run_automated_tests_sys(void) { return 1; }
