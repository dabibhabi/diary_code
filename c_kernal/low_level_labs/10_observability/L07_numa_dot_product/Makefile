# L07 — NUMA effects on dot

NM_ROOT  := ../../../../cpp/numerical_methods
NM_C_DIR := $(NM_ROOT)/c
INCLUDES := -I$(NM_ROOT)/include
CFLAGS   := -O3 -g -Wall -Wextra
LDFLAGS  := -lm -lnuma

.PHONY: build run-local run-remote run-interleave c2c-remote topology clean help

help:
	@echo "Targets:"
	@echo "  build           — compile numa_dot (TODO: write numa_dot.c)"
	@echo "  topology        — print numactl --hardware"
	@echo "  run-local       — pin to node 0 mem + node 0 cpu"
	@echo "  run-remote      — pin to node 1 mem + node 0 cpu (cross-socket)"
	@echo "  run-interleave  — interleave memory across all nodes"
	@echo "  c2c-remote      — perf c2c on the remote configuration"
	@echo "  clean           — remove artifacts"

numa_dot: numa_dot.c $(NM_C_DIR)/blas1.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^ $(LDFLAGS)

build: numa_dot

topology:
	numactl --hardware

run-local: build
	numactl --membind=0 --cpunodebind=0 ./numa_dot

run-remote: build
	numactl --membind=1 --cpunodebind=0 ./numa_dot

run-interleave: build
	numactl --interleave=all --cpunodebind=0 ./numa_dot

c2c-remote: build
	sudo perf c2c record -- numactl --membind=1 --cpunodebind=0 ./numa_dot
	sudo perf c2c report --stdio

clean:
	rm -f numa_dot perf.data perf.data.old
