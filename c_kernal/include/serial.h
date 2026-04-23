#ifndef KERNEL_SERIAL_H
#define KERNEL_SERIAL_H

#include "types.h"

// Serial (COM1) driver, useful for QEMU -serial stdio debug output.

#define SERIAL_COM1 0x3F8

// TODO: initialize the UART (disable interrupts, set baud divisor 38400,
//       8N1, enable FIFO, set DTR/RTS)
void serial_init(void);

// TODO: block until transmit holding register is empty, then write byte
void serial_putc(char c);

// TODO: write a null-terminated string via serial_putc
void serial_write(const char* s);

#endif
