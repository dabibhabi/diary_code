# K01 writeup

## Host
- Kernel:
- Major number assigned (from dmesg):

## Three-way oracle
| Source | First 64 bytes (hex) | Match |
|---|---|---|
| `/dev/nm_xorshift` |  | — |
| Userspace C (libnm_xorshift.so) |  |  |
| Python ctypes |  |  |

## Concurrency choice
- ☐ Single global state + spinlock
- ☐ Per-CPU state
- Why I chose this:

## Lessons
- Why kernel C is different from userspace C (your top three):
  1.
  2.
  3.

## What surprised me

## What I'd try next
- Add a private state per `open()` call (per-fd seeds)
- Implement `write()` to set the seed
- Add an ioctl for "get N bytes into a kernel-allocated bounce buffer"
