# K03 — `/proc/nm_stats` per-CPU counters

## Goal
Expose per-CPU statistics from K01/K02 via a `/proc/nm_stats` entry built with the `seq_file` API. Counters tracked per CPU: bytes generated, calls to read, average bytes per call.

## Why this is interesting
You'll learn:
- The `seq_file` API — the right way to print structured data from kernel to userspace
- **Per-CPU variables** (`DEFINE_PER_CPU`, `this_cpu_inc`) — lock-free counters that scale to many cores
- The classic kernel-coding tradeoff: per-CPU is great for write-heavy stats but reads must aggregate across all CPUs

This pattern shows up in `/proc/stat`, `/proc/meminfo`, `/proc/net/snmp` — once you've written one, you understand all of them.

## Prereqs
- K01 working (you have a module that does the actual work)
- Read `Documentation/filesystems/seq_file.rst`

## What to do
1. Add `DEFINE_PER_CPU(struct nm_cpu_stats, nm_stats);` to your module.
2. In the `read` callback, increment per-CPU counters with `this_cpu_inc(...)` and `this_cpu_add(...)` (no locks needed — preemption-safe).
3. Implement `proc_create_seq_private("nm_stats", 0444, NULL, &nm_stats_seq_ops, ...)`.
4. Define a `seq_operations` with `start/next/stop/show` that iterates CPUs.
5. The `show` function prints one row per CPU with all counters.
6. Test: `cat /proc/nm_stats` produces a table.
7. Run a `dd if=/dev/nm_xorshift` workload pinned to different CPUs (`taskset -c 0 dd ...` then `taskset -c 1 dd ...`) and watch which CPUs accumulate counts.

## Measurable outcome
- `cat /proc/nm_stats` prints one row per CPU
- Per-CPU counts match the CPU you pinned `dd` to (within ±1 due to occasional migration)
- Python notebook plots bar chart of bytes-per-CPU after a NUMA-aware load test

## Reuse
- K01 module with per-CPU stats grafted on
- `cpp/numerical_methods/c/xorshift.c` — unchanged

## Hint at next lab
K04 dives into the kernel's most subtle restriction: floating point requires explicit save/restore. Numerical kernels can't naively call `sqrt()`.

## References
See `references.md`.
