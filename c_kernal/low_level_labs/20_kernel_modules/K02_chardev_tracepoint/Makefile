# K02 — K01 + a static tracepoint

KDIR ?= /lib/modules/$(shell uname -r)/build
PWD  := $(shell pwd)

.PHONY: all clean load unload show-events trace-bpftrace trace-bcc help

help:
	@echo "Targets:"
	@echo "  all              — build module"
	@echo "  load             — sudo insmod"
	@echo "  unload           — sudo rmmod"
	@echo "  show-events      — list our tracepoints in tracefs"
	@echo "  trace-bpftrace   — bpftrace one-liner per-CPU counts"
	@echo "  trace-bcc        — run python/bindings.py BCC reader"
	@echo "  clean            — kernel build clean"

all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean

load: all
	sudo insmod nm_xorshift_tp.ko

unload:
	sudo rmmod nm_xorshift_tp

show-events:
	sudo cat /sys/kernel/tracing/available_events | grep nm_xorshift

trace-bpftrace:
	sudo bpftrace -e 'tracepoint:nm_xorshift:nm_xorshift_gen { @[args->cpu] = count(); }'

trace-bcc:
	sudo python3 python/bindings.py
