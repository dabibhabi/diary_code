# L05 — kprobe `__alloc_pages` while allocating dense matrices

## Goal
Write an eBPF program (BCC Python first, then libbpf-CO-RE if you want to graduate) that hooks `kprobe:__alloc_pages`. Run a workload that allocates dense N×N matrices for increasing N (then runs `cholesky_inplace` from `cpp/numerical_methods/c/cholesky_inplace.c`). Histogram the allocation orders and watch when the kernel starts handing out 2 MiB folios.

## Why this is interesting
You'll see the **page allocator** from inside. For small matrices, the allocator hands out single 4 KiB pages (order 0). At a threshold (governed by transparent hugepages and your `madvise` settings), the kernel starts using order-9 (2 MiB) folios. This is *enormously* important for HPC workloads because TLB pressure dominates large dense factorizations.

## Prereqs
- L04 done (you're comfortable with bpftrace)
- BCC installed (`bpfcc-tools` on Debian, `bcc-tools` on Arch)
- Optionally: clang + libbpf for the CO-RE path
- Read the `__alloc_pages` signature in `mm/page_alloc.c` of your kernel — the symbol may be `__alloc_pages_noprof` on recent kernels

## What to do
1. Write a workload `cholesky_workload.c` that allocates an N×N SPD matrix on the heap, fills it, runs `cholesky_inplace`, frees, increments N. Loop N over `[64, 128, 256, 512, 1024, 2048, 4096, 8192]`.
2. Write a BCC Python program (`python/bindings.py` is the entry point) that:
   - Attaches a kprobe on `__alloc_pages` (handle the suffix differences across kernels)
   - Stores `BPF_HISTOGRAM(orders)` keyed on `args->order`
   - Filters by `comm == "cholesky_workload"`
   - Prints the histogram every 5 seconds
3. Run workload + tracer together.
4. Watch the histogram shift from order 0 toward order 9 as N grows past `~2048`.

## Measurable outcome
- A histogram captured per N showing allocation-order distribution
- A clear inflection point where order-9 (2 MiB) allocations start appearing
- A note in the writeup connecting this to TLB pressure: large dense matrices benefit hugely from THP / hugepages

## Reuse
- `cpp/numerical_methods/c/cholesky_inplace.c` — workload's actual numerical work

## Hint at next lab
L06 zooms in on a different syscall path: `read(2)` going from VFS down to the block layer.

## References
See `references.md`.
