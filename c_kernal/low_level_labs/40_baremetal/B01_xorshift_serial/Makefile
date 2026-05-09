# B01 — xorshift over COM1
#
# This Makefile drives the parent c_kernal/test_project build (which uses i686-elf).
# QEMU gets COM1 piped to a Unix socket so the python harness can read it.

KERNEL_DIR := ../../../test_project
KERNEL_ELF := $(KERNEL_DIR)/build/kernel.elf
COM1_SOCK  := /tmp/com1
QEMU       := qemu-system-i386
QEMU_FLAGS := -kernel $(KERNEL_ELF) \
              -serial unix:$(COM1_SOCK),server,nowait \
              -nographic -no-reboot

.PHONY: build run harness test clean help

help:
	@echo "Targets:"
	@echo "  build    — build the bare-metal kernel via test_project's Makefile"
	@echo "  run      — boot in QEMU; COM1 → /tmp/com1 socket"
	@echo "  harness  — start the python harness reading /tmp/com1"
	@echo "  test     — run pytest oracle"

build:
	$(MAKE) -C $(KERNEL_DIR) all

run: build
	$(QEMU) $(QEMU_FLAGS)

harness:
	python3 python/bindings.py

test:
	pytest python/

clean:
	$(MAKE) -C $(KERNEL_DIR) clean
	rm -f $(COM1_SOCK)
