#include "io.h"

unsigned char keyboard_read();
char scancode_to_ascii(unsigned char scancode);
void print(const char* str);
void print_char(char c);
void shell_backspace();

void kernel_main() {
    print("ZiggyOS Active CLI Shell\n-> ");

    while(1) {
        unsigned char scancode = keyboard_read();
        char ascii = scancode_to_ascii(scancode);
        
        if (ascii != 0) {
            if (ascii == '\n') {
                print("\n-> ");
            } 
            else if (ascii == '\b') {
                shell_backspace(); // Process destructive backspace on screen
            } 
            else {
                print_char(ascii); // Print normal text letters
            }
        }
    }
}
