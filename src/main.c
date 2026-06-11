#include "io.h"
unsigned char keyboard_read();
char scancode_to_ascii(unsigned char scancode);
void print(const char* str);
void print_char(char c);
void shell_backspace();
void shell_clear();
char cmd_buffer[256];
int cmd_index = 0;
void process_command() {
	print("n");
	if (cmd_buffer[0] == 'r' \&\& cmd_buffer[1] == 'e' \&\& cmd_buffer[2] == 'b' \&\& cmd_buffer[3] == 'o' \&\& cmd_buffer[4] == 'o' \&\& cmd_buffer[5] == 't') {
		print("Rebooting...n");
		outb(0x64, 0xFE);
	} else if (cmd_buffer[0] == 'c' \&\& cmd_buffer[1] == 'l' \&\& cmd_buffer[2] == 'e' \&\& cmd_buffer[3] == 'a' \&\& cmd_buffer[4] == 'r') {
		shell_clear();
	} else if (cmd_buffer[0] == 'h' \&\& cmd_buffer[1] == 'e' \&\& cmd_buffer[2] == 'l' \&\& cmd_buffer[3] == 'p') {
		print("ZiggyOS Commands:n  help    - Commands listn  clear   - Wipe screenn  reboot  - Restartn  teal    - Teal spectrumn  emerald - Emerald spectrumn-> ");
	} else if (cmd_buffer[0] == 't' \&\& cmd_buffer[1] == 'e' \&\& cmd_buffer[2] == 'a' \&\& cmd_buffer[3] == 'l') {
		extern char current_color;
		current_color = 0x03;
		print("Electric Teal theme loaded.n-> ");
	} else if (cmd_buffer[0] == 'e' \&\& cmd_buffer[1] == 'm' \&\& cmd_buffer[2] == 'e' \&\& cmd_buffer[3] == 'r' \&\& cmd_buffer[4] == 'a' \&\& cmd_buffer[5] == 'l' \&\& cmd_buffer[6] == 'd') {
		extern char current_color;
		current_color = 0x0A;
		print("Emerald Spectrum theme loaded.n-> ");
	} else {
		print("Unknown command.n-> ");
	}
}
void kernel_main() {
	print("ZiggyOS Custom Spectrum Interfacen-> ");
	while(1) {
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
