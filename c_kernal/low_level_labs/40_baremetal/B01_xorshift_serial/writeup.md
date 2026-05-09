# B01 writeup

## Build/boot
- Toolchain: i686-elf-gcc ?
- QEMU:
- Kernel size (text+data): ?? bytes

## Output
```
(paste 16 hex lines from COM1)
```

## Three-way oracle
- ☐ Bare-metal == userspace C
- ☐ Userspace C == Python pyxorshift
- (If any mismatched: where in the algorithm, and why)

## Uniformity tests
| Test | Statistic | p-value | Reject H0? |
|---|---|---|---|
| Byte chi-square (256 bins) |  |  |  |
| KS vs Uniform[0, 2^64) |  |  |  |

## What surprised me
- (E.g., serial init was longer than the algorithm)

## What I'd try next
- B02 — interactive serial REPL
- Add VGA output alongside serial
- Implement a simple wait-for-keypress loop
