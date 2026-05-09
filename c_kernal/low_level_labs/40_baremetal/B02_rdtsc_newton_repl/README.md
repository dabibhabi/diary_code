# B02 — RDTSC-bracketed fixed-point Newton, in a serial REPL

## Goal
Add a tiny serial-input REPL to your bare-metal kernel. Commands:
- `cycles` → run fixed-point Newton sqrt N=10000 times, print mean cycles/op via RDTSC
- `sqrt <int>` → return Q32.32 sqrt of the input
- `help` → list commands
- `quit` → halt

## Why this is interesting
Cycle-accurate timing on bare metal — no scheduler interference, no syscall overhead, no CFS wakeups, no preemption — is the cleanest microbenchmark possible. You'll see numbers tighter than anything achievable in userspace.

## Prereqs
- B01 done (`serial_init`, `serial_putc`, `serial_write`)
- `serial_getc` to read input
- Drivers/keyboard.c can wait — serial input is enough for now

## What to do
1. Add `serial_getc()` to `c_kernal/test_project/drivers/serial.c`. Polling: wait until LSR data-ready, read RBR.
2. Implement a simple line buffer reader (`serial_readline`).
3. RDTSC inline asm: `__asm__ volatile("rdtsc" : "=a"(lo), "=d"(hi))`. Use `rdtscp` if available; serializing matters.
4. Port `nm_newton_sqrt_q32` (the same algorithm from K04, freestanding-friendly).
5. REPL loop:
   - read a line
   - parse first word as command
   - dispatch to handler
   - print result
6. Host harness (`python/bindings.py`): connect to COM1 socket, send commands, parse responses.
7. Run `cycles` 100 times, plot the histogram of per-op cycle counts.

## Measurable outcome
- REPL responds to all four commands
- `cycles` reports a stable per-op count (jitter likely < 5%, much better than userspace)
- Histogram from Python looks like a tight peak with a small tail
- Comparison: same Newton iteration in userspace under `taskset -c 3 nice -n -20` shows wider jitter

## Reuse
- B01 serial driver
- Newton iteration from K04 (or author it here)

## Hint at next lab
B03 prints to VGA text mode and adds Simpson's rule for an integer integral.

## References
See `references.md`.
