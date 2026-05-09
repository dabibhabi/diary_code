# B03 — integer Simpson's rule, output to VGA text mode

## Goal
Compute `∫₀¹ x² dx` (analytic answer: 1/3) using composite Simpson's rule, **using only integer arithmetic** (Q32.32 fixed-point). Print the result to VGA text mode at 0xB8000. Compare against `scipy.integrate.quad` from the host.

## Why this is interesting
- VGA text mode is the original "hello world" surface — every bare-metal tutorial starts here. You'll have it after this lab.
- Integer-only Simpson is a real engineering exercise: scaling to avoid overflow while preserving precision. Embedded systems (without FPUs) do this all the time.

## Prereqs
- B01 + B02 done (you have a working build, serial output)
- Read OSDev wiki "VGA Text Mode"

## What to do
1. Implement `c_kernal/test_project/drivers/vga.c`:
   - `vga_clear()` — fill 80×25 cells with space + default attribute
   - `vga_putc(char)` — track cursor, scroll on row 25
   - `vga_write_str(s)`
   - `vga_write_q32(u64)` — print Q32.32 as `<int>.<frac6>` without libc
2. Implement integer Simpson over `[a, b]`, `n` even subintervals:
   - All inputs in Q32.32
   - Watch for intermediate overflow on `f(x) * weight`; consider 64-bit accumulator
   - For `f(x) = x²`, `f` itself can be done in integer math: x² in Q32.32 is `(x * x) >> 32`
3. Compute for `n = 4, 16, 64, 256` subintervals, print result for each.
4. Host-side: `scipy.integrate.quad(lambda x: x*x, 0, 1)` reference. Diff vs your kernel result.
5. Plot convergence: error vs n (should be O(1/n^4) for Simpson).

## Measurable outcome
- VGA shows four lines like `n=  4: 0.333344`, `n= 16: 0.333333`, ...
- Python notebook plots log-log error vs n with slope ≈ -4
- Q32.32 result agrees with scipy to ~6 decimal digits

## Reuse
- `c_kernal/test_project/drivers/vga.c` (currently TODO scaffold)
- B01/B02 serial output for the host harness to scrape
- A future `cpp/numerical_methods/src/quadrature/simpson.cpp` (currently stubbed) — author the integer twin here

## Hint at next lab
B04 sets up an IDT and PIT timer interrupt, taking you from "I can compute" to "I can preempt myself" — the core of any real OS kernel.

## References
See `references.md`.
