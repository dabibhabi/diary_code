# L03 — talk to the PMU yourself with `perf_event_open(2)`

## Goal
Reproduce a subset of `perf stat` by calling `perf_event_open(2)` directly. Wrap a saxpy loop from `cpp/numerical_methods/c/blas1.c` and print cycles + instructions yourself. Optionally use RDPMC for nanosecond-precision read.

## Why this is interesting
You'll learn the **actual mechanism** behind `perf`. It's a single syscall returning a file descriptor; you `read()` the fd to get counter values. RDPMC lets you skip the syscall entirely on each read — useful for sub-microsecond measurements inside a tight loop.

## Prereqs
- L01 + L02 done
- `kernel.perf_event_paranoid <= 1` (see `00_setup/`)
- Read `man 2 perf_event_open` end-to-end before coding

## What to do
1. Author `pmu_saxpy.c`. The flow:
   - `struct perf_event_attr` for `PERF_TYPE_HARDWARE / PERF_COUNT_HW_CPU_CYCLES`
   - `perf_event_open(&attr, /*pid=*/0, /*cpu=*/-1, /*group_fd=*/-1, 0)`
   - Same for `PERF_COUNT_HW_INSTRUCTIONS`, opened in the same group
   - `ioctl(group_fd, PERF_EVENT_IOC_RESET, PERF_IOC_FLAG_GROUP)`
   - `ioctl(group_fd, PERF_EVENT_IOC_ENABLE, PERF_IOC_FLAG_GROUP)`
   - Run saxpy from `cpp/numerical_methods/c/blas1.c`
   - `ioctl(... DISABLE ...)`
   - `read(group_fd, ...)` returns a struct with both counters
2. Print IPC.
3. Compare against `perf stat -e cycles,instructions ./pmu_saxpy`. They should agree to within ±1%.
4. (Stretch) RDPMC: enable `attr.use_rdpmc`, mmap the fd to get the user-space-readable counter, read with the `rdpmc` instruction.

## Measurable outcome
- Your `pmu_saxpy` prints cycles and instructions matching `perf stat` to ±1%.
- Python notebook (`python/notebook.ipynb`) sweeps N (vector length) and plots IPC vs N — you'll see a knee where the dataset stops fitting in L1.

## Reuse
- `cpp/numerical_methods/c/blas1.c` — the `saxpy` and `dot` routines

## Hint at next lab
L04 moves from "I read counters from inside my own program" to "I observe arbitrary kernel events from outside" via bpftrace.

## References
See `references.md`.
