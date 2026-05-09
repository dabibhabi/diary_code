# L02 — perf record + flame graph on a CPU Monte Carlo path simulation
#
# You'll author mc_paths.c in this folder. The Makefile builds it with frame
# pointers so `perf record --call-graph fp` works.

NM_ROOT     := ../../../../cpp/numerical_methods
NM_C_DIR    := $(NM_ROOT)/c
INCLUDES    := -I$(NM_ROOT)/include
CFLAGS      := -O2 -g -fno-omit-frame-pointer -Wall -Wextra
LDFLAGS     := -lm
FLAMEGRAPH  ?= $(HOME)/src/FlameGraph   # override: make FLAMEGRAPH=...

SRCS := mc_paths.c $(NM_C_DIR)/xorshift.c $(NM_C_DIR)/inv_normal_cdf.c
OBJS := $(notdir $(SRCS:.c=.o))

.PHONY: build run record flame clean help

help:
	@echo "Targets:"
	@echo "  build   — compile mc_paths (TODO: write mc_paths.c)"
	@echo "  run     — run mc_paths once"
	@echo "  record  — perf record + perf script + stackcollapse"
	@echo "  flame   — produce flame.svg"
	@echo "  clean   — remove build artifacts and flame.svg"

mc_paths: $(SRCS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^ $(LDFLAGS)

build: mc_paths

run: build
	./mc_paths

record: build
	perf record -F 997 -g --call-graph fp -- ./mc_paths
	perf script > out.perf

flame: record
	$(FLAMEGRAPH)/stackcollapse-perf.pl out.perf | $(FLAMEGRAPH)/flamegraph.pl > flame.svg
	@echo "Open flame.svg in a browser."

clean:
	rm -f mc_paths out.perf perf.data perf.data.old flame.svg
