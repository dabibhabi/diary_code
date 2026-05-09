# B04 writeup

## Build/boot
- IDT entries populated:
- PIC remapped to: 0x20, 0x28
- PIT reload value: ?? (target 100 Hz at 1.193182 MHz)

## Histogram (1024 ticks)
- Mean: ?? cycles
- Stddev: ??
- p99:
- Max outlier:
- Plot: see `python/notebook.ipynb`

## Comparison to userspace 100 Hz timer
| Source | mean | stddev | p99 | max |
|---|---|---|---|---|
| Bare-metal PIT |  |  |  |  |
| Userspace `sleep(0.01)` (pinned) |  |  |  |  |

## Where do tail-latency events come from?
- QEMU host scheduler:
- VM-exits / hypervisor:

## What surprised me

## What I'd try next
- Switch from PIT to APIC timer
- Add nested interrupt support
- Run on real hardware (not QEMU) for true bare-metal numbers
