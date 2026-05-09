# RESOURCES — curated reading for `low_level_labs/`

Per-lab `references.md` files are **subsets** of this index. If a link appears here, it means I've used it personally or it's a primary source. Treat the rest of the internet as supplementary.

## Books (in priority order — read at least chapters relevant to your active lab)

| Book | Why |
|---|---|
| _Linux Kernel Development_, Robert Love (3rd ed., 2010) | Best on-ramp to kernel C. Old kernel version (2.6) but the model still holds. Covers LKMs, kthreads, locking, memory. |
| _Systems Performance_, Brendan Gregg (2nd ed., 2020) | The reference for L-track. Methodologies (USE, RED), tools, and how to reason about Linux performance. |
| _BPF Performance Tools_, Brendan Gregg (2019) | Companion volume. Every chapter is "here's a problem, here's the bpftrace one-liner." |
| _Operating Systems: Three Easy Pieces_, Arpaci-Dusseau (free PDF at `pages.cs.wisc.edu/~remzi/OSTEP/`) | The OS theory you'll want behind B-track. Free. |
| _Computer Architecture: A Quantitative Approach_, Hennessy & Patterson (6th ed.) | What the PMU counters _mean_. Read if `perf stat` output ever feels like a wall of jargon. |
| _Linux Device Drivers_, Corbet/Rubini/Kroah-Hartman (3rd ed., LWN-mirrored, free) | Older but still the structural reference for K-track. |

## Online docs / wikis

- **kernel.org/doc/html/latest/** — official kernel docs
  - `admin-guide/perf-security.rst` — what `perf_event_paranoid` does
  - `trace/` — ftrace, kprobes, uprobes, tracepoints
  - `bpf/` — eBPF
  - `core-api/` — per-CPU vars, locking, RCU
- **OSDev wiki** (`wiki.osdev.org`) — primary B-track reference. Multiboot, IDT, PIT, GDT, RDTSC, paging.
- **ebpf.io** + **libbpf-bootstrap** (GitHub `libbpf/libbpf-bootstrap`) — minimal CO-RE templates.
- **perf.wiki.kernel.org** — `perf` tutorial, event lists, RDPMC.
- **Brendan Gregg's blog** (`brendangregg.com`) — flame graphs, off-CPU profiling, USE method, NUMA.
- **Julia Evans' zines** (`wizardzines.com`) — gentle primers for `perf`, `strace`, eBPF.

## Tutorials / source trees worth reading directly

- **The Linux Kernel Module Programming Guide** (`sysprog21/lkmpg` on GitHub) — currently maintained walkthrough; primary K-track tutorial.
- Linux source itself (clone or browse `elixir.bootlin.com`):
  - `samples/bpf/` — official eBPF examples
  - `samples/trace_events/` — tracepoint examples
  - `tools/perf/` — `perf` source
  - `Documentation/trace/ftrace.rst`
- **iovisor/bcc** + **bpftrace** (GitHub) — copy/paste recipes.
- **xv6** (MIT, `github.com/mit-pdos/xv6-public`) — small reference kernel; compare design choices to `c_kernal/test_project/`.
- **Phil Opp's "Writing an OS in Rust"** — different language, same B-track concepts. Excellent prose on bringup.

## Standards / specs (primary sources — consult when something is mysterious)

- **Multiboot1 spec** — `gnu.org/software/grub/manual/multiboot/multiboot.html` (already used by `c_kernal/`).
- **Intel SDM** (`software.intel.com/sdm`) — Vol. 3B Ch. 18–19 (PMU, counters), Vol. 1 Ch. 10 (FPU, x87, SSE state).
- **AMD APM** Vol. 2 — equivalent if you're on AMD; PMU events differ.
- **`perf_event_open(2)`** man page — primary source for L03.
- **`bpf(2)`** man page — eBPF syscall reference.

## Talks / videos (one-time orientation)

- Brendan Gregg, "Linux Performance Tools" (USENIX LISA, multiple years) — orientation.
- Liz Rice, "A Beginner's Guide to eBPF" (KubeCon talks, multiple years).
- "Writing your first kernel module" walkthroughs on YouTube — pick one with a recent kernel (≥ 5.10).

## Python interop

- **Python `ctypes`** — `docs.python.org/3/library/ctypes.html`. Primary tool for K/B userspace clients.
- **`cffi`** — `cffi.readthedocs.io`. Better when you have headers and want auto-generated bindings.
- **pybind11** — `pybind11.readthedocs.io`. Already used by `cpp/numerical_methods/python/`.
- **iovisor/bcc Python frontend** — `github.com/iovisor/bcc/tree/master/tools` for canonical examples.
- **`pyperf`** — `pyperf.readthedocs.io`. Statistically rigorous Python timing.
- **`pyserial`** — `pyserial.readthedocs.io`. B-track host harness over QEMU `-serial unix:...`.
- **NumPy + SciPy testing** — `numpy.testing.assert_array_equal`, `scipy.stats.kstest`, `scipy.stats.chisquare`.

## Watch out

- Kernel APIs drift fast. Anything you read pre-5.10 about cgroups, BPF, or memory might be wrong. Cross-check against current `Documentation/`.
- `perf_event_paranoid` semantics changed in recent kernels — read the kernel.org page for your kernel version, not a blog post from 2018.
- BCC and libbpf-CO-RE are different tools. BCC compiles BPF on the host; CO-RE compiles once and relocates. For learning, BCC is friendlier; for "production", CO-RE.
