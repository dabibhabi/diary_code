# B04 — IDT + PIT timer; measure scheduling jitter

## Goal
Set up an Interrupt Descriptor Table, register a handler for IRQ0 (PIT timer), program PIT at 100 Hz. On every tick, capture RDTSC and store the delta from the previous tick. After 1024 ticks, dump the histogram via serial. Plot the distribution from Python.

## Why this is interesting
This is **the** entry point to OS internals. After this lab you understand:
- IDT setup (segment selector, gate type, DPL, present bit)
- PIC remapping (real-mode default IRQs collide with x86 exceptions)
- ISR stub assembly (saving registers, calling C, EOI)
- PIT programming (channel 0, mode 3, 16-bit reload value)
- The "tick" concept underlying every preemptive scheduler

The numerical-methods angle is the **histogram** of inter-tick deltas — a real distribution measurement on a ground-truth timer source.

## Prereqs
- B01–B03 done
- Read OSDev wiki: "Interrupt Descriptor Table", "8259 PIC", "PIT", "Interrupts tutorial"

## What to do
1. Implement `c_kernal/test_project/cpu/idt.c` (and matching `idt.h`):
   - `struct idt_entry` (8 bytes) and `struct idt_ptr`
   - `idt_set_gate(num, base, sel, flags)`
   - `idt_install()` — fill IDT, `lidt`
2. Implement PIC remap in `cpu/pic.c`: PIC1 → 0x20, PIC2 → 0x28.
3. Implement PIT in `drivers/pit.c`: program channel 0, mode 3, 100 Hz.
4. Write the IRQ0 ISR stub in NASM:
   - Push registers, call `pit_handler(void)` in C, pop registers, EOI, `iret`
5. `pit_handler`:
   - RDTSC, store delta from last tick
   - Increment tick counter
   - At 1024 ticks: disable interrupts, dump histogram via serial, `hlt`
6. Histogram: 64 bins covering ~`(expected_tsc_per_tick - 10%, +10%)` range, count per bin.
7. Host harness: parse the histogram dump, plot.

## Measurable outcome
- 1024 ticks observed; histogram dumped; QEMU halts cleanly
- Distribution looks roughly normal (or has a heavy tail if QEMU host is busy)
- Plot annotates: mean, stddev, p99, longest outlier
- Compared to a "userspace 100 Hz timer" (e.g., Python `time.sleep(0.01)` loop) — bare metal should show much tighter jitter

## Reuse
- B01 serial output for histogram dump
- (No numerical_methods reuse — this is foundational OS work)

## Hint at next track
You've now built a kernel that handles interrupts. The natural next step would be a real scheduler — but that's a 100-hour project, not a lab. Instead, try expanding `cpp/numerical_methods/` itself: pick a stubbed chapter (quadrature, ODE, optimization) and implement it with the perspective you've built across L+K+B.

## References
See `references.md`.
