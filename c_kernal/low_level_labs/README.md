# low_level_labs — bridging numerical methods and the kernel

Self-paced labs that tie the numerical-methods work in `cpp/numerical_methods/` to what's actually happening underneath: CPU counters, kernel allocators, page tables, ring 0, interrupts, perf, eBPF.

You're a cloud / data / devops engineer who already reads C and C++ comfortably. These labs are **observation-first**: you'll spend more time reading `perf` output and `dmesg` than writing code. Each lab is self-contained — pick one, work it, fill in `writeup.md`.

> Nothing in this folder ships. It's a learning sandbox. Break things, instrument things, write down what surprised you.

## Tracks

| Track | Slug | What you do | Touch the kernel? |
|---|---|---|---|
| **L** — Linux observability | `10_observability/` | Run `perf`, `bpftrace`, `ftrace`, `perf_event_open(2)` against numerical workloads | No, observe only |
| **K** — Linux kernel modules | `20_kernel_modules/` | Build and `insmod` tiny out-of-tree LKMs that expose numerical primitives | Yes, ring 0 (Linux) |
| **B** — Bare-metal extensions | `40_baremetal/` | Add numerical kernels to the i686-elf hobby kernel under `c_kernal/test_project/` | Yes, ring 0 (your own kernel) |

Track L is the gentlest entry point. Track K teaches you the kernel C dialect (no FP without `kernel_fpu_begin()`, per-CPU vars, locking). Track B is "you own everything from boot to print."

## Lab map

```
10_observability/
├── L01_perf_stat_crtp           — IPC and miss rates on the CRTP microbench
├── L02_perf_record_mc           — flame graph a Monte Carlo path simulation
├── L03_perf_event_open          — talk to the PMU yourself; reproduce `perf stat` (Python opt-in)
├── L04_bpftrace_oneliners       — page faults, off-CPU, sched_switch on a heat-PDE solver
├── L05_libbpf_alloc_pages       — kprobe `__alloc_pages` while allocating dense matrices (Python opt-in via BCC)
├── L06_ftrace_csv_load          — function_graph the VFS path on `read(2)`
└── L07_numa_dot_product         — quantify cross-socket bandwidth penalty (Python opt-in)

20_kernel_modules/
├── K01_xorshift_chardev         — /dev/nm_xorshift returning random bytes (Python client)
├── K02_chardev_tracepoint       — add a static tracepoint to K01 (Python BCC reader)
├── K03_proc_nm_stats            — /proc/nm_stats with per-CPU counters (Python parser)
├── K04_kernel_fpu_newton        — fixed-point + kernel_fpu_begin/end Newton sqrt (Python compare)
└── K05_netlink_saxpy            — pedagogical anti-example: kernel saxpy via netlink (Python client)

40_baremetal/
├── B01_xorshift_serial          — random uint64s over COM1 (Python uniformity tests)
├── B02_rdtsc_newton_repl        — RDTSC-bracketed fixed-point Newton in a serial REPL (Python histograms)
├── B03_ring0_simpson            — integer Simpson's rule, print to VGA (Python diff vs scipy)
└── B04_pit_jitter               — IDT + PIT @ 100 Hz; jitter histogram (Python distribution fit)
```

## How to do a lab

1. `cd` into the lab folder.
2. Read `README.md` — it says what you're trying to learn and what success looks like.
3. Read `references.md` — read at least one of the linked resources before touching code.
4. Look at `Makefile` — see what targets are skeletoned in.
5. Implement. The point of these labs is that **you write the code**. The skeleton is a starting point, not a solution.
6. Run. Capture output. Take screenshots of flame graphs / charts.
7. Fill in `writeup.md` — what you observed, what surprised you, what you'd do next.
8. (If `python/` exists) `pytest python/` — your three-way oracle. Kernel/bare-metal output should match the userspace C reference and the Python ctypes computation.

## Prereqs (one-time)

See `00_setup/README.md` for the full host setup. Quick version:

- Linux host (any distro; commands shown for Debian/Ubuntu and Arch)
- `linux-headers-$(uname -r)`, `build-essential`, `clang`, `make`
- `linux-tools-common linux-tools-$(uname -r)` (perf), `bpftrace`, `bpfcc-tools` (Linux track)
- `i686-elf-gcc`, `i686-elf-binutils`, `nasm`, `qemu-system-i386`, `grub-mkrescue`, `xorriso` (B-track — same toolchain `c_kernal/test_project/` already needs)
- Python 3.10+, then `python -m venv .venv && source .venv/bin/activate && pip install -e .` from this directory (uses `pyproject.toml`)
- A few sysctls relaxed: `kernel.perf_event_paranoid=0`, `kernel.kptr_restrict=0`. Don't run a public-facing server with these set.

## Reusable from `cpp/numerical_methods/`

These labs **port primitives** from the existing C TUs rather than reinventing them. Direct candidates:

- `cpp/numerical_methods/c/xorshift.c` → K01, K02, B01
- `cpp/numerical_methods/c/blas1.c` → L03 (saxpy/dot), K05, L07
- `cpp/numerical_methods/c/inv_normal_cdf.c` → L02 (Monte Carlo workload)
- `cpp/numerical_methods/c/tridiag.c` → L04 (heat PDE inner loop)
- `cpp/numerical_methods/c/cholesky_inplace.c` → L05 (allocate large SPD matrix, factorize)
- `cpp/numerical_methods/include/c_api/nm_inline.h` → extend with `NM_KERNEL` / `NM_BARE` macros

## Curated resources

See `RESOURCES.md` for the full reading list. Each lab's `references.md` is a 3–7 link subset focused on that lab.

## Out of scope

- Embedded firmware (Cortex-M, RISC-V) — separate future track
- Userland drivers (io_uring, VFIO, DPDK) — separate future track
- Anything that requires `sudo` and runs across the whole system (no `bcc-tools/execsnoop` in production use, etc.)
