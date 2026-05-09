#include "keyboard.h"

// TODO: small ring buffer for decoded keypresses
// TODO: scancode set 1 -> ASCII translation table (shift/caps handling later)

void keyboard_init(void) {
    // TODO: mask/unmask IRQ1 on the PIC, install IDT entry,
    //       flush any pending byte from port 0x60
}

char keyboard_read(void) {
    // TODO: pop one char off the ring buffer, return 0 if empty
    return 0;
}

void keyboard_handle_irq(void) {
    // TODO: read scancode from 0x60, translate, push into ring buffer,
    //       send EOI (0x20 -> port 0x20) to PIC
}
