# L04 — bpftrace one-liners against a heat-PDE solver

## Goal
Use `bpftrace` to capture page faults, scheduler switches, and off-CPU time on a CPU port of `cuda/pde_heat_explicit.cu`. No code changes to the workload — bpftrace attaches to existing kernel tracepoints from outside.

## Why this is interesting
This is the "free observability" lab. You haven't recompiled anything. You haven't added log statements. You haven't added counters. You just attach to kernel tracepoints from a one-line script and watch the workload reveal itself.

## Prereqs
- `bpftrace` installed and working: `sudo bpftrace -e 'BEGIN { printf("ok\n"); exit(); }'`
- A CPU heat-PDE binary you can run for ≥ 30 seconds (so traces have time to accumulate samples)

## What to do
Write a CPU heat-PDE driver in this folder (or reuse one from L02 if you build a separate `pde_heat.c`). Then run each of these and capture the output:

```bash
# 1. Page-fault count by process
sudo bpftrace -e 'tracepoint:exceptions:page_fault_user { @[comm] = count(); }'

# 2. Off-CPU time histogram for our process
sudo bpftrace -e 'kprobe:finish_task_switch { @sw[pid] = nsecs; }
                  kretprobe:schedule /@sw[pid]/ { @[comm] = hist(nsecs - @sw[pid]); delete(@sw[pid]); }'

# 3. Syscall counts
sudo bpftrace -e 'tracepoint:raw_syscalls:sys_enter /comm == "pde_heat"/ { @[args->id] = count(); }'

# 4. Block-I/O latency (zero if entirely in-memory)
sudo bpftrace -e 'tracepoint:block:block_rq_issue { @s[args->dev, args->sector] = nsecs; }
                  tracepoint:block:block_rq_complete /@s[args->dev, args->sector]/ {
                      @lat = hist((nsecs - @s[args->dev, args->sector]) / 1000); delete(@s[args->dev, args->sector]); }'

# 5. CPU migrations
sudo bpftrace -e 'tracepoint:sched:sched_migrate_task /args->comm == "pde_heat"/ { @ = count(); }'
```

## Measurable outcome
A `traces/` directory in this lab folder with five output files. Plus a short writeup interpreting each:
- Did page faults happen, and when (startup only, or steady-state)?
- Was your process ever off-CPU? Why?
- Which syscalls did it actually make?
- Block-I/O latency (probably zero — pure compute)
- Did the scheduler migrate your process? If so, did you set CPU affinity?

## Reuse
- `cpp/numerical_methods/c/tridiag.c` if you do an implicit (Crank-Nicolson) variant
- The CUDA kernel `cuda/pde_heat_explicit.cu` for algorithm shape

## Hint at next lab
L05 takes you from `bpftrace` recipes to your own libbpf-CO-RE program — same observability, more control, deployable.

## References
See `references.md`.
