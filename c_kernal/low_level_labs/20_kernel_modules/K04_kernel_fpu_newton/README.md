# K04 — Newton's method in kernel C: fixed-point AND `kernel_fpu_begin/end`

## Goal
Implement Newton-Raphson for sqrt **two ways** in a kernel module:
1. **Fixed-point** (Q32.32) — never touches the FPU. Always safe in kernel context.
2. **Floating-point** wrapped in `kernel_fpu_begin()` / `kernel_fpu_end()` — exposes the rules around FPU state preservation.

Compare results, performance, and complexity. Expose both via ioctls on a char device.

## Why this is interesting
The kernel's most-asked-about restriction: **you can't just call `sqrt()`**. The CPU's FPU state belongs to whichever userspace thread was last running. If a kernel function dirties FPU registers without saving them, you corrupt that thread's float values silently — a horrible bug.

`kernel_fpu_begin()` saves and `kernel_fpu_end()` restores. They have nontrivial cost and disable preemption. This is why the kernel mostly avoids FP and uses fixed-point or integer math.

## Prereqs
- K01–K03 done (you can build a module and expose ioctls or sysfs)
- Read `arch/x86/include/asm/fpu/api.h` and grep for `kernel_fpu_begin` in the kernel source

## What to do
1. Implement `nm_newton_sqrt_q32(u64 x_q32) -> u64` — fixed-point, no FP. Port the iteration from the future `cpp/numerical_methods/c/newton_fixed.c` (or write it fresh; algorithm is just `x_{n+1} = (x_n + a/x_n) / 2`).
2. Implement `nm_newton_sqrt_f64(double x) -> double` — FP, wrap with `kernel_fpu_begin/end`. Note: kernel typically forbids `double` parameters in normal kernel API; expose via ioctl with `__user`-buffered scalars.
3. Expose both via a char device + ioctls (`NM_IOC_SQRT_Q32`, `NM_IOC_SQRT_F64`).
4. Userspace test program (or ctypes from Python) calls both and compares.
5. Benchmark inside the module (kernel-side `ktime_get_ns()`) for both paths over N=1e6 inputs. Report ns/op for each.

## Measurable outcome
- Both paths return correct sqrts (matching `math.sqrt` to within fixed-point quantization)
- Fixed-point benchmark is faster than FP-wrapped version (kernel_fpu_begin has a non-trivial cost)
- Notebook plots both error vs input value (fixed-point shows quantization steps)

## Reuse
- `cpp/numerical_methods/c/blas1.c` style for the userspace reference build
- A future `cpp/numerical_methods/c/newton_fixed.c` (currently stubbed) — feel free to author it as a side-effect of this lab and PR it into the parent library

## Hint at next lab
K05 closes the K-track with a deliberate anti-example: doing saxpy in the kernel via netlink to learn why "kernel does the work" is usually the wrong answer.

## References
See `references.md`.
