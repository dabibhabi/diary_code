# K05 — generic netlink saxpy (anti-example)

KDIR ?= /lib/modules/$(shell uname -r)/build
PWD  := $(shell pwd)

.PHONY: all clean load unload bench help

help:
	@echo "Targets:"
	@echo "  all     — build module"
	@echo "  load    — sudo insmod"
	@echo "  unload  — sudo rmmod"
	@echo "  bench   — run python bench: kernel netlink vs userspace numpy"

all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean

load: all
	sudo insmod nm_netlink_saxpy.ko

unload:
	sudo rmmod nm_netlink_saxpy

bench:
	python3 python/bindings.py
