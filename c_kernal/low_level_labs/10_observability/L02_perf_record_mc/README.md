# L02 — flame graph a Monte Carlo simulation

## Goal
Use `perf record` + flame graphs to find the hottest function in a CPU port of `cpp/numerical_methods/cuda/monte_carlo_paths.cu`. Confirm `xorshift64` and `inv_normal_cdf` dominate (they should — every path needs new randomness).

## Why this is interesting
You'll see the difference between **counter-based summaries** (L01) and **sampling-based attribution** (this lab). Sampling tells you _where_, counters tell you _how much_. Both matter; flame graphs are the visualization that makes _where_ easy to see.

## Prereqs
- L01 done (you're comfortable with `perf` basics)
- Brendan Gregg's `FlameGraph` repo cloned: `git clone https://github.com/brendangregg/FlameGraph` somewhere on your system

## What to do
1. Write a small CPU host program in this lab folder (e.g., `mc_paths.c`) that runs N=1M Monte Carlo paths under geometric Brownian motion, using `xorshift64` for the uniform stream and `inv_normal_cdf` to convert to standard normals. Reuse the implementations from `cpp/numerical_methods/c/xorshift.c` and `cpp/numerical_methods/c/inv_normal_cdf.c`.
2. Build with `-O2 -fno-omit-frame-pointer -g`.
3. `perf record -F 997 -g --call-graph fp -- ./mc_paths`.
4. `perf script | <path-to-FlameGraph>/stackcollapse-perf.pl | <path-to-FlameGraph>/flamegraph.pl > flame.svg`.
5. Open `flame.svg` in a browser. Identify the widest leaves.
6. Verify: `xorshift64` and `inv_normal_cdf` are at the top of the CPU time list.

## Measurable outcome
- A `flame.svg` checked into the lab folder.
- `writeup.md` lists the top 5 leaf functions with their share of CPU time.
- An explanation of why the path loop body is what it is (1 xorshift call + 1 inv_normal_cdf call + a multiply + an add).

## Reuse
- `cpp/numerical_methods/c/xorshift.c` — `static inline` random gen
- `cpp/numerical_methods/c/inv_normal_cdf.c` — Beasley-Springer-Moro or whatever the lib uses

## Hint at next lab
L03 takes you below `perf stat` itself and asks: "what would I have to do to read these counters with no `perf` binary at all?"

## References
See `references.md`.
