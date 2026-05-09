# L04 — bpftrace one-liners against a heat-PDE workload

NM_ROOT  := ../../../../cpp/numerical_methods
NM_C_DIR := $(NM_ROOT)/c
INCLUDES := -I$(NM_ROOT)/include
CFLAGS   := -O2 -g -Wall -Wextra
LDFLAGS  := -lm

.PHONY: build run trace-faults trace-offcpu trace-syscalls trace-bio trace-migrate clean help

help:
	@echo "Targets:"
	@echo "  build           — compile pde_heat (TODO: write pde_heat.c)"
	@echo "  run             — run pde_heat"
	@echo "  trace-faults    — page-fault count by process"
	@echo "  trace-offcpu    — off-CPU time histogram"
	@echo "  trace-syscalls  — syscall counts for pde_heat"
	@echo "  trace-bio       — block-I/O latency"
	@echo "  trace-migrate   — CPU-migration count"
	@echo "  clean           — remove build artifacts"

pde_heat: pde_heat.c $(NM_C_DIR)/tridiag.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^ $(LDFLAGS)

build: pde_heat

run: build
	./pde_heat

# All tracing targets assume `pde_heat` is running in another shell.

trace-faults:
	sudo bpftrace -e 'tracepoint:exceptions:page_fault_user { @[comm] = count(); }'

trace-offcpu:
	sudo bpftrace -e 'kprobe:finish_task_switch { @sw[pid] = nsecs; } \
	                   kretprobe:schedule /@sw[pid]/ { @[comm] = hist(nsecs - @sw[pid]); delete(@sw[pid]); }'

trace-syscalls:
	sudo bpftrace -e 'tracepoint:raw_syscalls:sys_enter /comm == "pde_heat"/ { @[args->id] = count(); }'

trace-bio:
	sudo bpftrace -e 'tracepoint:block:block_rq_issue { @s[args->dev, args->sector] = nsecs; } \
	                   tracepoint:block:block_rq_complete /@s[args->dev, args->sector]/ { \
	                       @lat = hist((nsecs - @s[args->dev, args->sector]) / 1000); \
	                       delete(@s[args->dev, args->sector]); }'

trace-migrate:
	sudo bpftrace -e 'tracepoint:sched:sched_migrate_task /args->comm == "pde_heat"/ { @ = count(); }'

clean:
	rm -f pde_heat
