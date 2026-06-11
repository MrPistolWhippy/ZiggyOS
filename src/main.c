
void kernel_main() {
    print("ZiggyOS Initialization Complete.\n");
    while(1) {
        unsigned char scancode = keyboard_read();
        if (scancode == 0x1C) { // Enter key scancode
            print("\n[Command Submitted]");
        }
    }
}
