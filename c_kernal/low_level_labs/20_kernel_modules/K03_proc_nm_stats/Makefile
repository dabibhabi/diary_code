# K03 — /proc/nm_stats with per-CPU counters

KDIR ?= /lib/modules/$(shell uname -r)/build
PWD  := $(shell pwd)

.PHONY: all clean load unload show stress help

help:
	@echo "Targets:"
	@echo "  all     — build module"
	@echo "  load    — sudo insmod"
	@echo "  unload  — sudo rmmod"
	@echo "  show    — cat /proc/nm_stats"
	@echo "  stress  — pin dd to different CPUs to populate per-CPU counts"

all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean

load: all
	sudo insmod nm_xorshift_stats.ko

unload:
	sudo rmmod nm_xorshift_stats

show:
	cat /proc/nm_stats

stress:
	@for cpu in 0 1 2 3; do \
	  echo "--- pinned to CPU $$cpu ---"; \
	  taskset -c $$cpu dd if=/dev/nm_xorshift bs=8 count=10000 of=/dev/null status=none; \
	done
	@echo
	@cat /proc/nm_stats
