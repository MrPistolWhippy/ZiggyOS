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
