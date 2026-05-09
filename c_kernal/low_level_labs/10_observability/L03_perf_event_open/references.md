# L03 references

- `man 2 perf_event_open` — primary source. Read every section.
- Linux source `tools/perf/` — production-quality user of this API; lots to learn from
- "perf_event_open by example" blog posts (search) — many short tutorials exist; pick one with code that compiles on your kernel
- Intel SDM Vol. 3B, Ch. 19.4 — RDPMC instruction semantics
- `cpp/numerical_methods/c/blas1.c` — your saxpy/dot reference
- LWN: "perf events on Linux" series (search "lwn perf events") — historical context for why the API looks like it does
