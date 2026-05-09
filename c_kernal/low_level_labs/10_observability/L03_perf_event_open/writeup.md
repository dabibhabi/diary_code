# L03 writeup

## Host
- CPU model:
- Kernel:
- `perf_event_paranoid`:

## Counter agreement
| Source | cycles | instructions | IPC |
|---|---|---|---|
| Our `pmu_saxpy` |  |  |  |
| `perf stat` |  |  |  |
| Δ |  |  |  |

## IPC vs N (from `python/notebook.ipynb`)
- Knee at N ≈ ?? (where dataset exceeds L1d)
- L1d size on this CPU: ?? KiB

## Did RDPMC work?
- ☐ Used the syscall path
- ☐ Tried `rdpmc` from userspace
- Tradeoffs you noticed:

## What surprised me
