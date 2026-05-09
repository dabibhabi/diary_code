# B02 — RDTSC Newton REPL

KERNEL_DIR := ../../../test_project
KERNEL_ELF := $(KERNEL_DIR)/build/kernel.elf
COM1_SOCK  := /tmp/com1
QEMU       := qemu-system-i386
QEMU_FLAGS := -kernel $(KERNEL_ELF) \
              -serial unix:$(COM1_SOCK),server,nowait \
              -nographic -no-reboot

.PHONY: build run repl bench test clean help

help:
	@echo "Targets:"
	@echo "  build  — build the bare-metal kernel"
	@echo "  run    — boot in QEMU"
	@echo "  repl   — interactive REPL via python harness"
	@echo "  bench  — run 100x cycles command, plot histogram"

build:
	$(MAKE) -C $(KERNEL_DIR) all

run: build
	$(QEMU) $(QEMU_FLAGS)

repl:
	python3 python/bindings.py --interactive

bench:
	python3 python/bindings.py --bench

test:
	pytest python/

clean:
	$(MAKE) -C $(KERNEL_DIR) clean
	rm -f $(COM1_SOCK)
