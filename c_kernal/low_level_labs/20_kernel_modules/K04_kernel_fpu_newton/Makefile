# K04 — Newton sqrt in kernel: fixed-point vs FP-wrapped

KDIR ?= /lib/modules/$(shell uname -r)/build
PWD  := $(shell pwd)

.PHONY: all clean load unload bench test help

help:
	@echo "Targets:"
	@echo "  all     — build module"
	@echo "  load    — sudo insmod"
	@echo "  unload  — sudo rmmod"
	@echo "  bench   — run kernel-side bench, dump results via printk"
	@echo "  test    — run pytest oracle"

all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean

load: all
	sudo insmod nm_newton.ko

unload:
	sudo rmmod nm_newton

bench:
	@echo "1" | sudo tee /sys/module/nm_newton/parameters/run_bench
	dmesg | tail -20

test:
	pytest python/
