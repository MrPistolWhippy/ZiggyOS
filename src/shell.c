void shell_print_color(const char* str, char color) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] == '\n') { 
            cursor_pos = ((cursor_pos / 80) + 1) * 80; 
        } 
        else { 
            video_memory[cursor_pos] = (color << 8) | str[i]; 
            cursor_pos++; 
        }
    }
}

void shell_backspace() {
    if (cursor_pos > 0) {
        cursor_pos--; // Move cursor back one step
        video_memory[cursor_pos] = (0x0F << 8) | ' '; // Clear character with blank space
    }
}

void shell_clear() {
	for (int i = 0; i < 2000; i++) {
		*(video_memory + i) = (0x0F << 8) | ' ';
	}
	cursor_pos = 0;
}

char current_color = 0x03;
void shell_print_color(const char* str, char color) {
	for (int i = 0; str[i] != '0'; i++) {
		if (str[i] == 'n') {
			cursor_pos = ((cursor_pos / 80) + 1) * 80;
		} else {
			*(video_memory + cursor_pos) = (color << 8) | str[i];
			cursor_pos++;
		}
	}
}
void print(const char* str) {
	shell_print_color(str, current_color);
}
