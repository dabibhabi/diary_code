# L06 — ftrace function_graph on `read(2)`

## Goal
Use ftrace's `function_graph` tracer to follow a `read(2)` call from the VFS layer all the way down to the block layer (or page-cache hit short-circuit). Workload: load a CSV of sample data for a future interpolation routine.

## Why this is interesting
Most engineers treat `read(2)` as an atom. ftrace shows you it isn't: VFS → file system (ext4/btrfs) → page cache → block layer → device driver. You'll see the **first** read take ~10× the time of subsequent reads to the same file (cold vs warm page cache).

## Prereqs
- ftrace mounted at `/sys/kernel/tracing` (default on most kernels)
- Root access (or `tracefs` permissions configured)
- A non-trivial CSV file to load (1+ MB — generate one with `python -c "..."`)

## What to do
1. Author `csv_loader.c` that opens a CSV, `read()`s it in 64 KiB chunks until EOF, parses to floats. Don't use `mmap` — we want explicit `read(2)`.
2. Drop caches before the cold run: `echo 3 | sudo tee /proc/sys/vm/drop_caches`.
3. Set up ftrace:
   ```bash
   cd /sys/kernel/tracing
   echo function_graph | sudo tee current_tracer
   echo 'vfs_read'     | sudo tee set_graph_function
   echo csv_loader     | sudo tee set_ftrace_pid
   echo 1 > tracing_on
   ```
4. Run `csv_loader` once (cold) and once (warm).
5. `cat /sys/kernel/tracing/trace > cold.trace` (after the cold run, then again for warm).
6. Diff. The cold trace will descend into `submit_bio` etc.; the warm trace will return from page cache without any block-layer calls.

## Measurable outcome
- `cold.trace` and `warm.trace` files in this lab
- A short writeup naming five functions in the cold path that do not appear in the warm path (your evidence for "page cache served the read")

## Reuse
None — this is a pure observability lab. The CSV is just a pretext to issue real reads.

## Hint at next lab
L07 takes you across NUMA boundaries and shows why the *physical placement* of your data matters as much as the access pattern.

## References
See `references.md`.
