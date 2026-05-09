# L05 writeup

## Host
- Kernel:
- THP setting: `cat /sys/kernel/mm/transparent_hugepage/enabled`
- BCC version:

## Allocation-order histogram per N
| N | dominant order | mean alloc size | #allocs |
|---|---|---|---|
| 64 |  |  |  |
| 256 |  |  |  |
| 1024 |  |  |  |
| 4096 |  |  |  |
| 8192 |  |  |  |

## Where did order-9 (2 MiB) first appear?
N = ??

## Did `madvise(MADV_HUGEPAGE)` change anything? (optional)

## What surprised me
