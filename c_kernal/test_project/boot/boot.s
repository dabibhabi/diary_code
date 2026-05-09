; boot.s - Multiboot header and entry point
;
; TODO: Define the multiboot1 header (magic, flags, checksum) so GRUB
;       will recognize this binary as a kernel.
; TODO: Reserve a small stack in .bss and set %esp to the top of it.
; TODO: Push the multiboot info pointer and magic number onto the stack,
;       then call kmain. Halt the CPU afterwards.

bits 32

section .multiboot
; TODO: multiboot header goes here

section .bss
; TODO: reserve stack

section .text
global _start
extern kmain

_start:
    ; TODO: set up stack pointer
    ; TODO: call kmain
    ; TODO: cli / hlt loop
    jmp $
