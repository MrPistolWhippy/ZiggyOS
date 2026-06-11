#include "io.h"
unsigned char keyboard_read();
int keyboard_has_key();
char scancode_to_ascii(unsigned char scancode);
void print(const char* str);
void print_char(char c);
void shell_backspace();
void shell_clear();
void init_gdt();
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
void kernel_main() {
	init_gdt();
	shell_clear();
	print("ZiggyOS Custom Segmented Shelln-> ");
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
