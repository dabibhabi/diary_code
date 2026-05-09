# L01 references

- `perf-stat(1)` man page — primary source for event syntax and `--repeat` usage
- Brendan Gregg, "CPU Flame Graphs" intro (`brendangregg.com/cpuperf.html`) — what IPC and branch misses actually tell you
- Intel SDM Vol. 3B Ch. 18 — the underlying PMU events behind `cycles`, `instructions`, `branch-misses`
- `cpp/numerical_methods/include/patterns/` — read the CRTP and virtual implementations side by side before measuring
- `perf list` (run on your host) — full event catalog for your specific CPU
