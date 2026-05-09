# K04 references

- `arch/x86/include/asm/fpu/api.h` — `kernel_fpu_begin/end` declaration and constraints.
- LWN, "Floating-point in the kernel" (Corbet, search for it) — the canonical writeup.
- Q-format primer: `en.wikipedia.org/wiki/Q_(number_format)` — fixed-point representation.
- Kernel source: drivers that legitimately use FP (search for `kernel_fpu_begin` — examples in crypto/, drivers/gpu/drm/i915/, sound/).
- "Newton's method" Wikipedia page — algorithm reference.
- Brendan Gregg, "Linux Performance" — for context on why FP-in-kernel is a perf pessimization.
