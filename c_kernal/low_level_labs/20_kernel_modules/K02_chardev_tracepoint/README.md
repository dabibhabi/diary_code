# K02 — add a static tracepoint to K01

## Goal
Add a custom static tracepoint to the K01 module that fires every time a `read()` produces output. Watch the tracepoint from `bpftrace` and from a BCC Python program.

## Why this is interesting
Static tracepoints are the **right** way to expose internal events to userspace observability. They have:
- Stable ABI (kprobes can break across kernel versions; tracepoints don't, if you don't change the format)
- Negligible cost when no observer is attached (a 5-byte NOP)
- Structured arguments (no parsing, unlike printk)

This is how production kernel code exposes interesting events — every Linux subsystem you've heard of (sched, block, net, mm) does this.

## Prereqs
- K01 working (loads, `/dev/nm_xorshift` produces output)

## What to do
1. Create `tracepoints/nm_xorshift.h` defining a `TRACE_EVENT(nm_xorshift_gen, ...)`. The args should include `bytes`, `state`, `cpu`.
2. Add `#define CREATE_TRACE_POINTS` + include to `nm_xorshift.c`.
3. In your `read` handler, call `trace_nm_xorshift_gen(bytes, state, smp_processor_id())` after generating output.
4. Update `Kbuild` so the trace header is on the include path.
5. Rebuild, reload.
6. Verify it appears: `sudo cat /sys/kernel/tracing/available_events | grep nm_xorshift`.
7. Watch via bpftrace: `sudo bpftrace -e 'tracepoint:nm_xorshift:nm_xorshift_gen { @[args->cpu] = count(); }'`
8. Run `dd if=/dev/nm_xorshift bs=8 count=1000 of=/dev/null`. Watch counts accumulate per CPU.
9. Write a BCC Python reader (`python/bindings.py`) that opens the tracepoint and prints structured events live.

## Measurable outcome
- `/sys/kernel/tracing/available_events` lists `nm_xorshift:nm_xorshift_gen`
- bpftrace output shows per-CPU counts that match `dd`'s byte total ÷ 8
- BCC Python program prints live events without observable overhead in K01

## Reuse
- K01 module as the base
- `samples/trace_events/` in the kernel source as reference

## Hint at next lab
K03 takes the per-CPU counter idea to its native home: a `/proc/nm_stats` entry exposing per-CPU counters via the seq_file API.

## References
See `references.md`.
