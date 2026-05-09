# B03 — integer Simpson's rule, VGA output

KERNEL_DIR := ../../../test_project
KERNEL_ELF := $(KERNEL_DIR)/build/kernel.elf
COM1_SOCK  := /tmp/com1
QEMU       := qemu-system-i386

# Use SDL or VNC display so VGA text shows. For headless capture, use -d.
QEMU_GUI   := -kernel $(KERNEL_ELF) -serial unix:$(COM1_SOCK),server,nowait
QEMU_HEAD  := -kernel $(KERNEL_ELF) -serial unix:$(COM1_SOCK),server,nowait \
              -display none -monitor none

.PHONY: build run run-headless test clean help

help:
	@echo "Targets:"
	@echo "  build         — build kernel"
	@echo "  run           — boot in QEMU with VGA window"
	@echo "  run-headless  — boot in QEMU, VGA off, COM1 only"
	@echo "  test          — pytest oracle"

build:
	$(MAKE) -C $(KERNEL_DIR) all

run: build
	$(QEMU) $(QEMU_GUI)

run-headless: build
	$(QEMU) $(QEMU_HEAD)

test:
	pytest python/

clean:
	$(MAKE) -C $(KERNEL_DIR) clean
	rm -f $(COM1_SOCK)
