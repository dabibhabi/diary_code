# L04 writeup

## Host
- Kernel:
- bpftrace version:
- pde_heat config: grid=??x??, steps=??

## Trace summaries
| Trace | Headline finding |
|---|---|
| page-fault count |  |
| off-CPU histogram |  |
| syscall counts |  |
| block-I/O latency |  |
| CPU migrations |  |

## What surprised me

## What I'd try next
- Add an `mmap`-backed grid and re-run trace-faults
- Pin to a single CPU and re-run trace-migrate (should drop to zero)
- Try `bcc/tools/biolatency.py` for nicer block-I/O output
