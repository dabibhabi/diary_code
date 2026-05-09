#ifndef KERNEL_KEYBOARD_H
#define KERNEL_KEYBOARD_H

#include "types.h"

// PS/2 keyboard driver (port 0x60 data, 0x64 status/command).

// TODO: register IRQ1 handler, set up scancode->ascii table
void keyboard_init(void);

// TODO: return next key from internal ring buffer, or 0 if empty (non-blocking)
char keyboard_read(void);

// TODO: IRQ1 ISR entry — called from the IDT stub
void keyboard_handle_irq(void);

#endif
