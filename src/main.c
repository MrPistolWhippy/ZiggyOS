#include "io.h"
unsigned char keyboard_read();
int keyboard_has_key();
char scancode_to_ascii(unsigned char scancode);
void print(const char* str);
void print_char(char c);
void shell_backspace();
void shell_clear();
void init_gdt();
void init_idt();
void pic_remap();
unsigned long timer_ticks = 0;
char cmd_buffer[256];
int cmd_index = 0;
void print_uptime() {
	unsigned long seconds = timer_ticks / 10000;
	print("System Uptime: ");
	char buf[16];
	int i = 0;
	if(seconds == 0) { print("0sn"); return; }
	while(seconds > 0) {
		buf[i++] = '0' + (seconds % 10);
		seconds /= 10;
	}
	while(i > 0) { print_char(buf[--i]); }
	print("sn");
}
void process_command() {
	print("n");
	if (cmd_buffer[0] == 'r' \&\& cmd_buffer[1] == 'e' \&\& cmd_buffer[2] == 'b' \&\& cmd_buffer[3] == 'o' \&\& cmd_buffer[4] == 'o' \&\& cmd_buffer[5] == 't') {
		print("Rebooting...n");
		outb(0x64, 0xFE);
	} else if (cmd_buffer[0] == 's' && cmd_buffer[1] == 'h') {
		outw(0xB004, 0x2000); outw(0x604, 0x2000);
		while(1);
	} else if (cmd_buffer[0] == 'p' && cmd_buffer[1] == 's') {
		extern void print_process_status();
	} else if (cmd_buffer[0] == 'm' && cmd_buffer[1] == 's' && cmd_buffer[2] == 'g') {
		extern void ipc_send(const char* m);
		extern void ipc_receive();
	} else if (cmd_buffer == 'b' && cmd_buffer == 'e' && cmd_buffer == 'e' && cmd_buffer == 'p') {
		extern void mmu_trigger_beep();
		print("Sounding hardware speaker...n");
		mmu_trigger_beep();
		ipc_send("Hello from Task 0!");
		ipc_receive();
	} else if (cmd_buffer == 'b' && cmd_buffer == 'e' && cmd_buffer == 'e' && cmd_buffer == 'p') {
		extern void mmu_trigger_beep();
		print("Sounding hardware speaker...n");
		mmu_trigger_beep();
		print_process_status();
	} else if (cmd_buffer[0] == 'm' && cmd_buffer[1] == 's' && cmd_buffer[2] == 'g') {
		extern void ipc_send(const char* m);
		extern void ipc_receive();
	} else if (cmd_buffer == 'b' && cmd_buffer == 'e' && cmd_buffer == 'e' && cmd_buffer == 'p') {
		extern void mmu_trigger_beep();
		print("Sounding hardware speaker...n");
		mmu_trigger_beep();
		ipc_send("Hello from Task 0!");
		ipc_receive();
	} else if (cmd_buffer == 'b' && cmd_buffer == 'e' && cmd_buffer == 'e' && cmd_buffer == 'p') {
		extern void mmu_trigger_beep();
		print("Sounding hardware speaker...n");
		mmu_trigger_beep();
		print("-> ");
	} else if (cmd_buffer[0] == 'c' \&\& cmd_buffer[1] == 'l' \&\& cmd_buffer[2] == 'e' \&\& cmd_buffer[3] == 'a' \&\& cmd_buffer[4] == 'r') {
		shell_clear();
	} else if (cmd_buffer[0] == 'u' \&\& cmd_buffer[1] == 'p' \&\& cmd_buffer[2] == 't' \&\& cmd_buffer[3] == 'i' \&\& cmd_buffer[4] == 'm' \&\& cmd_buffer[5] == 'e') {
		print_uptime();
		print("-> ");
	} else if (cmd_buffer[0] == 'm' \&\& cmd_buffer[1] == 'e' \&\& cmd_buffer[2] == 'm') {
		extern void* malloc(unsigned int size);
		extern void free(void* ptr, unsigned int size);
		void* block = malloc(4096);
		print("Heap Allocated/Freed 4KB Block At: 0x");
		unsigned int addr = (unsigned int)block;
		char buf[16];
		int i = 0;
		while(addr > 0) { unsigned int rem = addr % 16; if(rem < 10) buf[i++] = '0' + rem; else buf[i++] = 'A' + (rem - 10); addr /= 16; }
		while(i > 0) { print_char(buf[--i]); }
		free(block, 4096);
		print("n-> ");
	} else {
		print("Unknown command.n-> ");
	}
}
void kernel_main() {
	init_gdt();
	init_idt();
	pic_remap();
	shell_clear();
	__asm__ __volatile__("sti");
	print("ZiggyOS Active PIC Enabled Shelln-> ");
	while(1) {
		timer_ticks++;
		if (keyboard_has_key()) {
			unsigned char scancode = keyboard_read();
			char ascii = scancode_to_ascii(scancode);
			if (ascii != 0) {
				if (ascii == 'n') {
					cmd_buffer[cmd_index] = '0';
					process_command();
					cmd_index = 0;
				} else if (ascii == 'b') {
					if (cmd_index > 0) { cmd_index--; shell_backspace(); }
				} else {
					if (cmd_index < 255) { cmd_buffer[cmd_index++] = ascii; print_char(ascii); }
				}
			}
		}
	}
}
