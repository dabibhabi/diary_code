# L03 — perf_event_open() wrapper around saxpy

NM_ROOT  := ../../../../cpp/numerical_methods
NM_C_DIR := $(NM_ROOT)/c
INCLUDES := -I$(NM_ROOT)/include
CFLAGS   := -O2 -g -Wall -Wextra
LDFLAGS  := -lm

SRCS := pmu_saxpy.c $(NM_C_DIR)/blas1.c

.PHONY: build run compare clean help

help:
	@echo "Targets:"
	@echo "  build   — compile pmu_saxpy (TODO: write pmu_saxpy.c)"
	@echo "  run     — run pmu_saxpy"
	@echo "  compare — run ours and `perf stat` and diff"
	@echo "  clean   — remove build artifacts"

pmu_saxpy: $(SRCS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^ $(LDFLAGS)

build: pmu_saxpy

run: build
	./pmu_saxpy

compare: build
	@echo "=== our reading ==="
	@./pmu_saxpy
	@echo "=== perf stat ==="
	@perf stat -e cycles,instructions ./pmu_saxpy 2>&1

clean:
	rm -f pmu_saxpy
