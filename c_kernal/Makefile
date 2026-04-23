# Makefile - builds a bootable kernel ISO
#
# TODO: fill in CC/AS/LD (e.g. i686-elf-gcc cross toolchain).
# TODO: add iso/qemu/clean targets.
# TODO: wire CFLAGS for freestanding: -ffreestanding -nostdlib -fno-builtin
#       -fno-stack-protector -Wall -Wextra -m32 -Iinclude

CC      := i686-elf-gcc
AS      := nasm
LD      := i686-elf-ld

CFLAGS  := -ffreestanding -nostdlib -fno-builtin -m32 -Wall -Wextra -Iinclude
ASFLAGS := -f elf32
LDFLAGS := -T linker.ld -nostdlib -m elf_i386

C_SRCS  := kernel/kernel.c \
           drivers/vga.c drivers/serial.c drivers/keyboard.c \
           mm/pmm.c \
           lib/string.c

ASM_SRCS := boot/boot.s

OBJS    := $(C_SRCS:.c=.o) $(ASM_SRCS:.s=.o)

KERNEL  := kernel.elf
ISO     := kernel.iso

.PHONY: all iso run clean

all: $(KERNEL)

# TODO: link rule — $(LD) $(LDFLAGS) -o $@ $^
$(KERNEL): $(OBJS)
	@echo "TODO: link $(OBJS) into $@"

# TODO: compile rules for .c and .s
%.o: %.c
	@echo "TODO: compile $<"

%.o: %.s
	@echo "TODO: assemble $<"

# TODO: build a GRUB-bootable ISO via grub-mkrescue (needs iso/boot/grub/grub.cfg)
iso: $(KERNEL)
	@echo "TODO: grub-mkrescue -o $(ISO) iso/"

# TODO: qemu-system-i386 -kernel $(KERNEL) -serial stdio
run: $(KERNEL)
	@echo "TODO: qemu-system-i386 -kernel $(KERNEL) -serial stdio"

clean:
	rm -f $(OBJS) $(KERNEL) $(ISO)