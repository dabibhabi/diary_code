/* linker.ld - kernel link script
 *
 * TODO: load the kernel at the conventional 1 MiB mark (0x00100000).
 * TODO: place .multiboot first so GRUB finds the header within 8 KiB.
 * TODO: export `kernel_end` so pmm_init knows where free RAM begins.
 */

ENTRY(_start)

SECTIONS
{
    . = 1M;

    /* TODO: .multiboot (keep), .text, .rodata, .data, .bss sections */

    /* TODO: kernel_end = .; */
}
