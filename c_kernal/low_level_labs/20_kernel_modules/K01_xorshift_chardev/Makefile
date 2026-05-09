# K01 — out-of-tree kernel module: /dev/nm_xorshift
#
# Out-of-tree LKMs use the kernel's build system via M=$(PWD).
# `Kbuild` declares which sources compose the module.

KDIR ?= /lib/modules/$(shell uname -r)/build
PWD  := $(shell pwd)

.PHONY: all clean load unload test help

help:
	@echo "Targets:"
	@echo "  all     — build nm_xorshift.ko"
	@echo "  load    — sudo insmod"
	@echo "  unload  — sudo rmmod"
	@echo "  test    — read 64 bytes via xxd"
	@echo "  clean   — kernel build clean + remove tarballs"

all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean

load: all
	sudo insmod nm_xorshift.ko
	@echo "Now: ls -l /dev/nm_xorshift; dmesg | tail"

unload:
	sudo rmmod nm_xorshift

test:
	dd if=/dev/nm_xorshift bs=8 count=8 status=none | xxd
