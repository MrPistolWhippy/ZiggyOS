#ifndef IO_H
#define IO_H

static inline unsigned char inb(unsigned short port) {
    unsigned char ret;
    __asm__ __volatile__ ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

static inline void outb(unsigned short port, unsigned char val) {
    __asm__ __volatile__ ("outb %0, %1" : : "a"(val), "Nd"(port));
}

static inline unsigned short inw(unsigned short port) {
    unsigned short ret;
    __asm__ __volatile__ ("inw %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

static inline void outw(unsigned short port, unsigned short val) {
    __asm__ __volatile__ ("outw %0, %1" : : "a"(val), "Nd"(port));
}

void print(const char* str);
void print_char(char c);
void shell_clear();
void shell_backspace();

int keyboard_has_key();
unsigned char keyboard_read();
char scancode_to_ascii(unsigned char scancode);

void init_scheduler();
void create_task(void (*entry_point)(), void* stack_space);
void task_switch();
void pit_interrupt_scheduler_hook();
void print_process_status();
void background_worker_task();

void play_sound(unsigned int nFrequence);
void nosound();
void mmu_trigger_beep();

void ipc_send(const char* message);
void ipc_receive();

void mmu_hex_dump(void* start_addr);
void init_ramfs();

#endif

typedef struct __attribute__((__packed__)) {
	uint32_t timestamp;
	int32_t raw_temperature;
	uint32_t raw_pressure;
} atmospheric_payload_t;
