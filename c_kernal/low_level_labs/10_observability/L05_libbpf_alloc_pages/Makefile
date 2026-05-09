# L05 — eBPF kprobe on __alloc_pages while running Cholesky

NM_ROOT  := ../../../../cpp/numerical_methods
NM_C_DIR := $(NM_ROOT)/c
INCLUDES := -I$(NM_ROOT)/include
CFLAGS   := -O2 -g -Wall -Wextra
LDFLAGS  := -lm

.PHONY: build run trace-bcc trace-libbpf clean help

help:
	@echo "Targets:"
	@echo "  build         — compile cholesky_workload (TODO: write it)"
	@echo "  run           — run the workload"
	@echo "  trace-bcc     — run python/bindings.py BCC tracer (sudo)"
	@echo "  trace-libbpf  — TODO: libbpf-CO-RE alternative"
	@echo "  clean         — remove artifacts"

cholesky_workload: cholesky_workload.c $(NM_C_DIR)/cholesky_inplace.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^ $(LDFLAGS)

build: cholesky_workload

run: build
	./cholesky_workload

trace-bcc:
	sudo python3 python/bindings.py

trace-libbpf:
	@echo "TODO: build with bpftool gen skeleton + libbpf"

clean:
	rm -f cholesky_workload
