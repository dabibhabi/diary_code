#ifndef KERNEL_VGA_H
#define KERNEL_VGA_H

#include "types.h"

// VGA text-mode driver (0xB8000, 80x25)

enum vga_color {
    VGA_BLACK = 0,
    VGA_WHITE = 15,
    // TODO: fill out the remaining 14 colors
};

// TODO: clear the screen and reset cursor position
void vga_clear(void);

// TODO: write one character at current cursor, advancing position
void vga_putc(char c);

// TODO: write a null-terminated string
void vga_puts(const char* s);

// TODO: set the current fg/bg color attribute
void vga_set_color(uint8_t fg, uint8_t bg);

#endif
