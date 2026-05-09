# L07 — NUMA effects on a dot product

## Goal
Measure the bandwidth penalty of running `dot` (from `cpp/numerical_methods/c/blas1.c`) against a buffer pinned to a *remote* NUMA node vs a *local* one. Use `numactl` to control placement and `perf c2c` to confirm the cache-coherence traffic.

## Why this is interesting
A 1.5–3× slowdown on a single instruction (`vfmadd231pd` or similar) for no reason except that the bytes live across the QPI/UPI link. This is the kind of "invisible" performance issue that haunts HPC workloads on big multi-socket boxes.

## Prereqs
- A multi-socket NUMA machine (≥ 2 nodes). Confirm: `numactl --hardware` shows ≥ 2 nodes.
- If single-socket: this lab still teaches `numactl` and `perf c2c` syntax, but the bandwidth gap will be small.

## What to do
1. Author `numa_dot.c` that allocates a large vector (≥ L3 size; e.g. 256 MiB), fills it, calls `dot` from `cpp/numerical_methods/c/blas1.c` in a hot loop, and prints elapsed cycles + GB/s.
2. Build with `-O3`.
3. Run **local**: `numactl --membind=0 --cpunodebind=0 ./numa_dot`
4. Run **remote**: `numactl --membind=1 --cpunodebind=0 ./numa_dot`
5. Confirm with `perf c2c record -- numactl --membind=1 --cpunodebind=0 ./numa_dot` then `perf c2c report` — look for "remote" load events.
6. (Stretch) Run with `numactl --interleave=all` and observe partial recovery.

## Measurable outcome
- A table with three configs (local / remote / interleave) and their GB/s numbers
- `perf c2c` summary showing remote-load fraction
- Writeup answers: "If you can only choose one, pin memory or pin CPU?"

## Reuse
- `cpp/numerical_methods/c/blas1.c` — `dot` and `saxpy`

## Hint at next track
You've now exhausted the userspace observability bag. K-track moves you into ring 0 with out-of-tree LKMs.

## References
See `references.md`.
