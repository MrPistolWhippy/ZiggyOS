#include <stdint.h>
#include "font.h"
extern void put_pixel(int x, int y, uint8_t color);
int win_x = 60; int win_y = 60; int win_w = 200; int win_h = 100;
int win_visible = 1; int win_minimized = 0; uint8_t bg_color = 0;
void clear_vga_canvas(uint8_t color) {
    volatile uint8_t* s = (volatile uint8_t*)0xA0000;
    for (int i = 0; i < 64000; i++) s[i] = color;
}
void draw_char(char c, int x, int y, uint8_t color) {
    if (c < 0 || c > 127) return;
    for (int r = 0; r < 8; r++) {
        uint8_t b = font_sheet[(uint8_t)c][r];
        for (int col = 0; col < 8; col++) if (b & (0x80 >> col)) put_pixel(x + col, y + r, color);
    }
}
void draw_string(const char* str, int x, int y, uint8_t color) {
    while (*str) { draw_char(*str++, x, y, color); x += 8; }
}
void draw_window(int x, int y, int w, int h, const char* title) {
    if (!win_visible || win_minimized) return;
    for (int i = 0; i < w; i++) { for (int j = 0; j < h; j++) put_pixel(x + i, y + j, 7); }
    for (int i = 2; i < w - 2; i++) { for (int j = 2; j < 14; j++) put_pixel(x + i, y + j, 1); }
    for (int i = w - 14; i < w - 4; i++) { for (int j = 3; j < 13; j++) put_pixel(x + i, y + j, 4); }
    for (int i = w - 26; i < w - 16; i++) { for (int j = 3; j < 13; j++) put_pixel(x + i, y + j, 14); }
    draw_string(title, x + 6, y + 4, 15); draw_string("-", x + w - 23, y + 4, 0); draw_string("X", x + w - 12, y + 4, 15);
    for (int i = 6; i < w - 6; i++) { for (int j = 18; j < h - 6; j++) put_pixel(x + i, y + j, 0); }
    draw_string("ZOS TERM v1.0", x + 10, y + 22, 2); draw_string("calc_loop: RUN", x + 10, y + 36, 10);
}
void draw_desktop_taskbar(void) {
    for (int x = 0; x < 320; x++) { for (int y = 180; y < 200; y++) put_pixel(x, y, 8); }
    draw_string("START", 6, 186, 15);
    for (int x = 180; x < 200; x++) { for (int y = 184; y < 196; y++) put_pixel(x, y, 11); }
    draw_string("BG", 183, 186, 0);
    if (win_visible && win_minimized) {
        for (int x = 60; x < 110; x++) { for (int y = 182; y < 198; y++) put_pixel(x, y, 7); }
        draw_string("APP01", 66, 186, 0);
    }
}
