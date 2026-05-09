# L01 — perf stat on the CRTP microbench
#
# This Makefile does not build any source. It drives the parent build of
# cpp/numerical_methods/ and then invokes `perf stat` against the test binary.

NM_ROOT      := ../../../../cpp/numerical_methods
BIN_RELEASE  := $(NM_ROOT)/build/tests/test_crtp_dispatch
BIN_DEBUG    := $(NM_ROOT)/build-O0/tests/test_crtp_dispatch
PERF_EVENTS  := cycles,instructions,branches,branch-misses,L1-dcache-loads,L1-dcache-load-misses,LLC-loads,LLC-load-misses
CPU_PIN      ?= 3   # override with `make CPU_PIN=2 measure`

.PHONY: build build-debug measure measure-debug compare clean help

help:
	@echo "Targets:"
	@echo "  build          — build cpp/numerical_methods in Release"
	@echo "  build-debug    — build cpp/numerical_methods in Debug -O0"
	@echo "  measure        — perf stat on the Release binary"
	@echo "  measure-debug  — perf stat on the -O0 binary"
	@echo "  compare        — TODO: side-by-side IPC table"
	@echo "  clean          — TODO: remove any per-lab artifacts"

build:
	cd $(NM_ROOT) && cmake -B build -DCMAKE_BUILD_TYPE=Release && cmake --build build -j

build-debug:
	cd $(NM_ROOT) && cmake -B build-O0 -DCMAKE_BUILD_TYPE=Debug -DCMAKE_C_FLAGS=-O0 -DCMAKE_CXX_FLAGS=-O0 && cmake --build build-O0 -j

measure: build
	taskset -c $(CPU_PIN) perf stat --repeat 5 -e $(PERF_EVENTS) -- $(BIN_RELEASE)

measure-debug: build-debug
	taskset -c $(CPU_PIN) perf stat --repeat 5 -e $(PERF_EVENTS) -- $(BIN_DEBUG)

compare:
	@echo "TODO: capture stdout of measure and measure-debug, compute IPC delta, emit a Markdown table."

clean:
	@echo "Nothing to clean in this lab."
