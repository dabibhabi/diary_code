# B02 writeup

## Build/boot
- Kernel size:
- Boot to first prompt latency:

## REPL session transcript
```
> help
(paste)
> sqrt 100
(paste)
> cycles
(paste)
```

## Cycle stats (over 100 invocations of `cycles`)
- Mean: ?? cyc/op
- Stddev: ??
- p99: ??
- Min: ?? Max: ??

## Comparison to userspace
| Path | mean cyc/op | stddev | p99 |
|---|---|---|---|
| Bare-metal (this lab) |  |  |  |
| Userspace `taskset -c 3` |  |  |  |

## What surprised me

## What I'd try next
- Add `serializing` rdtsc with cpuid before/after
- Implement multi-iteration averaging inside the kernel
- Compare with TSC-deadline timer
