# B04 — IDT + PIT 100 Hz; jitter histogram

KERNEL_DIR := ../../../test_project
KERNEL_ELF := $(KERNEL_DIR)/build/kernel.elf
COM1_SOCK  := /tmp/com1
QEMU       := qemu-system-i386
QEMU_FLAGS := -kernel $(KERNEL_ELF) \
              -serial unix:$(COM1_SOCK),server,nowait \
              -display none -no-reboot

.PHONY: build run capture test clean help

help:
	@echo "Targets:"
	@echo "  build    — build kernel (must include cpu/idt.c, cpu/pic.c, drivers/pit.c)"
	@echo "  run      — boot in QEMU; halts after 1024 ticks"
	@echo "  capture  — run + harness to grab histogram"
	@echo "  test     — pytest oracle"

build:
	$(MAKE) -C $(KERNEL_DIR) all

run: build
	$(QEMU) $(QEMU_FLAGS)

capture: build
	$(QEMU) $(QEMU_FLAGS) &
	sleep 1
	python3 python/bindings.py
	wait

test:
	pytest python/

clean:
	$(MAKE) -C $(KERNEL_DIR) clean
	rm -f $(COM1_SOCK) histogram.json
