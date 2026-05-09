# B03 writeup

## Build/boot
- Kernel size:
- VGA initialized successfully?

## VGA output (transcribed)
```
n=   4: 0.????????
n=  16: 0.????????
n=  64: 0.????????
n= 256: 0.????????
```

## Convergence
| n | result (Q32.32 → float) | error vs 1/3 |
|---|---|---|
| 4 |  |  |
| 16 |  |  |
| 64 |  |  |
| 256 |  |  |

## Convergence rate
- Slope of log(error) vs log(n): ?? (Simpson predicts -4)

## Overflow encounters
- (Where I had to scale or widen the accumulator)

## What surprised me
