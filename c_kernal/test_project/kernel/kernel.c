#include "types.h"
#include "vga.h"
#include "serial.h"
#include "keyboard.h"
#include "pmm.h"

// kmain — called from boot.s once the stack is set up.
//
// Boot order matters:
//   1. serial_init  (so panics can log before VGA is usable)
//   2. vga_init/clear
//   3. pmm_init     (needs multiboot info passed from boot.s)
//   4. keyboard_init
//   5. enter idle/hlt loop
//
// TODO: accept (uint32_t magic, uint32_t mb_info) from boot.s and verify
//       magic == 0x2BADB002 before trusting mb_info.
 
void kmain(void) {
    // TODO: serial_init();
    // TODO: vga_clear();
    // TODO: vga_puts("hello, kernel\n");
    // TODO: pmm_init(mb_info);
    // TODO: keyboard_init();
    // TODO: sti + hlt loop
    for (;;) {
        // halt
    }
}

// TODO: kernel panic — print message via serial + vga, then cli/hlt
void kpanic(const char* msg) {
    (void)msg;
    for (;;) { /* halt */ }
}
