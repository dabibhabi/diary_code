#include "serial.h"

// TODO: inb/outb helpers (or pull them from a shared io.h)
static inline void outb(uint16_t port, uint8_t val) {
    // TODO: asm volatile ("outb %0, %1" :: "a"(val), "Nd"(port));
    (void)port; (void)val;
}

static inline uint8_t inb(uint16_t port) {
    // TODO: asm volatile ("inb %1, %0" : "=a"(r) : "Nd"(port));
    (void)port;
    return 0;
}

void serial_init(void) {
    // TODO: program COM1 registers (IER, LCR DLAB, divisor, FCR, MCR)
}

void serial_putc(char c) {
    // TODO: poll LSR bit 5 (THR empty) then outb(COM1, c)
    (void)c;
}

void serial_write(const char* s) {
    // TODO: loop serial_putc over s until '\0'
    (void)s;
}
