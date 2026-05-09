# K04 writeup

## Host
- CPU model:
- Kernel:

## Correctness
| input | fixed-point sqrt | FP sqrt | math.sqrt | rel err FX | rel err FP |
|---|---|---|---|---|---|
| 2.0 |  |  |  |  |  |
| 100.0 |  |  |  |  |  |
| 1e10 |  |  |  |  |  |

## Latency (ns/op, kernel-side)
| Method | mean | p50 | p99 |
|---|---|---|---|
| Fixed-point Q32.32 |  |  |  |
| FP w/ kernel_fpu_begin |  |  |  |

## Why is FP-wrapped slower?
- (Your answer — hint: count the work `kernel_fpu_begin` actually does)

## Would you ship FP-in-kernel?
- (Yes/no/why)

## What surprised me
