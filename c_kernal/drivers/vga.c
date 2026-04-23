#include "vga.h"

#define VGA_BUFFER ((volatile uint16_t*)0xB8000)
#define VGA_COLS   80
#define VGA_ROWS   25

// TODO: track cursor row/col and current color attribute in file-local state

void vga_clear(void) {
    // TODO: fill VGA_BUFFER with space characters using current attribute
    //       and reset cursor to (0, 0)
}

void vga_putc(char c) {
    // TODO: handle '\n', scroll when cursor passes last row,
    //       write (attr << 8) | c to VGA_BUFFER[row*VGA_COLS + col]
    (void)c;
}

void vga_puts(const char* s) {
    // TODO: loop vga_putc over s until '\0'
    (void)s;
}

void vga_set_color(uint8_t fg, uint8_t bg) {
    // TODO: store (bg << 4) | (fg & 0x0F) for subsequent writes
    (void)fg; (void)bg;
}
