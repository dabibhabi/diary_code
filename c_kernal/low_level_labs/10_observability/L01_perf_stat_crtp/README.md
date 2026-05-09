# L01 — `perf stat` on the CRTP microbench

## Goal
Use `perf stat` to measure IPC, branch mispredicts, and cache misses on `cpp/numerical_methods/tests/test_crtp_dispatch.cpp`. Compare release (`-O3`) vs unoptimized (`-O0`); compare CRTP dispatch vs virtual dispatch.

## Why this is interesting
CRTP is a "compile-time polymorphism" trick that the numerical_methods library uses to avoid v-table indirection in tight loops. The expected speedup is 3–10× on hot iterative solvers. **You should be able to *see* this in the PMU counters**: virtual dispatch shows higher branch-miss rate and lower IPC because the indirect call defeats the front-end branch predictor.

## Prereqs
- `00_setup/` complete
- `cpp/numerical_methods/` builds successfully: `cd cpp/numerical_methods && make build`

## What to do
1. Build the test binary in release: `cmake -DCMAKE_BUILD_TYPE=Release ... && cmake --build build` (the parent `Makefile` does this).
2. Locate the produced binary: `cpp/numerical_methods/build/test_crtp_dispatch` (path may vary).
3. Run: `perf stat -e cycles,instructions,branches,branch-misses,L1-dcache-loads,L1-dcache-load-misses,LLC-loads,LLC-load-misses ./test_crtp_dispatch`.
4. Repeat for a `-O0` build (separate build directory).
5. Pin to an isolated CPU: `taskset -c 3 perf stat ...` for stable numbers (see `00_setup/`).
6. Compute IPC = instructions / cycles. Compute branch-miss rate. Compare CRTP vs virtual paths.

## Measurable outcome
At `-O3`:
- IPC for CRTP path ≥ 3× IPC for virtual path
- Branch-miss rate clearly higher on virtual path
- L1-dcache-load-misses similar (algorithm is the same; only dispatch differs)

At `-O0`:
- The gap collapses or even inverts (instruction count dominates; predictor handles repeated indirect targets fine).

Write all of this in `writeup.md` with actual numbers from your machine.

## Reuse
None — `test_crtp_dispatch` is the workload as-is. No code in this lab; just measurement.

## Hint at next lab
L02 takes you from "summary counters" to "where in the program is the time?" via flame graphs.

## References
See `references.md`.
