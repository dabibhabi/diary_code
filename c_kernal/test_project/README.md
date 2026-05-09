# c_kernal

A tiny from-scratch x86 kernel. Scaffolding only — everything below is a TODO.

## TODO: Project goals
- [ ] Boot via GRUB (multiboot1) on `qemu-system-i386`
- [ ] Print to VGA text mode (0xB8000)
- [ ] Log to serial (COM1) for debugging
- [ ] Handle PS/2 keyboard input
- [ ] Physical memory manager (bitmap allocator)
- [ ] (stretch) paging + virtual memory
- [ ] (stretch) simple shell over keyboard input

## TODO: Directory layout
```
boot/       multiboot header + _start (asm)
kernel/     kmain and panic
drivers/    vga, serial, keyboard
mm/         physical memory manager
lib/        freestanding memcpy/memset/strlen
include/    public headers
linker.ld   link script (kernel loaded at 1 MiB)
Makefile    build / iso / run / clean
```

## TODO: Prerequisites
- [ ] i686-elf cross-compiler (`i686-elf-gcc`, `i686-elf-ld`)
- [ ] `nasm`
- [ ] `qemu-system-i386`
- [ ] `grub-mkrescue` + `xorriso` (for ISO)

## TODO: Build
```
make          # TODO: build kernel.elf
make iso      # TODO: produce kernel.iso via grub-mkrescue
make run      # TODO: boot in QEMU with serial piped to stdio
make clean    # remove build artifacts
```

## TODO: Implementation checklist
- [ ] `boot/boot.s` — multiboot header, stack, call `kmain`
- [ ] `kernel/kernel.c` — bring-up order + idle loop
- [ ] `drivers/vga.c` — clear, putc, puts, scroll, color
- [ ] `drivers/serial.c` — COM1 init + blocking write
- [ ] `drivers/keyboard.c` — IRQ1 ISR, scancode table, ring buffer
- [ ] `mm/pmm.c` — parse multiboot mmap, bitmap alloc/free
- [ ] `lib/string.c` — memset/memcpy/memcmp/strlen/strcmp
- [ ] `linker.ld` — sections + `kernel_end` symbol
- [ ] `Makefile` — real compile/link/iso/run rules
- [ ] `iso/boot/grub/grub.cfg` — GRUB menu entry

## TODO: Stretch goals
- [ ] GDT + IDT setup
- [ ] PIT-driven timer + preemption
- [ ] Paging (identity map + higher-half kernel)
- [ ] `kmalloc` / `kfree` on top of the PMM
- [ ] Tiny shell with builtins (`help`, `mem`, `clear`)

## TODO: References
- [ ] OSDev wiki — Bare Bones, Meaty Skeleton
- [ ] Intel SDM Vol. 3 (protected mode, paging)
- [ ] Multiboot1 specification
