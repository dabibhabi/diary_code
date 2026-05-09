# 00 — Index

Master TOC for the CUNY Student Management System design. Each chapter is self-contained; cross-references between chapters are explicit.

## Read order (recommended for newcomers)

1. **`01-overview.md`** — what we're building, what we're not, glossary.
2. **`02-architecture.md`** — the four layers + the data model + the database abstraction.
3. **`03-domain-model.md`** — members, roles, identity.
4. **`04-patterns.md`** — Strategy, Composite, plus the trade-offs of all three patterns.
5. **`05-flows.md`** — runtime sequences (enrollment, attendance, waitlist).
6. **`algorithms/`** — pick what's relevant; chapters are independent.
7. **`math/`** — the formulas backing the analytics; runnable via the notebooks (Phase 8).
8. **`conventions/`** — code style for C++ and Python, the build system.
9. **`interop/`** — how Python talks to C++ via nanobind.
10. **`25-todo-roadmap.md`** — phased build plan; check this off as you go.

## All chapters

| # | Path | Topic |
|---|------|-------|
| 00 | `00-index.md` | This file |
| 01 | `01-overview.md` | Goals, scope, glossary, actors, invariants |
| 02 | `02-architecture.md` | High-level architecture, ER diagram, repository pattern, mermaid+LaTeX setup |
| 03 | `03-domain-model.md` | CunyMember, Person, Campus, Roles, admin enum, lifecycle |
| 04 | `04-patterns.md` | Strategy (conflict detection), pattern trade-offs, composition over inheritance |
| 05 | `05-flows.md` | Enrollment, attendance recording, waitlist promotion sequences |
| 06 | `algorithms/06-prerequisite-graph.md` | DFS / BFS / Kahn / Tarjan / 2-SAT for prereqs |
| 07 | `algorithms/07-attendance-indexing.md` | Hashing strategies (chained, Robin Hood, Cuckoo, perfect, Bloom) |
| 08 | `algorithms/08-schedule-conflict.md` | Pairwise / interval tree / sweep-line / segment tree |
| 09 | `algorithms/09-waitlist-queues.md` | FIFO / binary heap / Fibonacci heap / pairing heap |
| 10 | `algorithms/10-streaming-aggregation.md` | Welford, reservoir sampling, count-min, HyperLogLog, t-digest |
| 11 | `algorithms/11-caching-memoization.md` | LRU cache, Union-Find, on-disk indexes |
| 12 | `math/12-statistics.md` | Welford derivation, Wilson CI, paired t-test, beta-binomial |
| 13 | `math/13-probability.md` | Birthday paradox, Poisson, M/M/1, geometric drop-out |
| 14 | `math/14-information-theory.md` | Entropy, KL divergence, mutual information, Huffman |
| 15 | `math/15-combinatorics.md` | Linear extensions, Catalan, inclusion-exclusion, Stirling |
| 16 | `math/16-optimization.md` | LP, min-cost flow, Hungarian, IP, simulated annealing |
| 17 | `math/17-linear-algebra-graphs.md` | PageRank, spectral clustering, Hopcroft-Karp, Dinic's |
| 18 | `conventions/18-cpp-style.md` | C++17/20 idioms, smart pointers, error handling |
| 19 | `conventions/19-python-style.md` | PEP 8, ruff/mypy/pytest/hypothesis, Protocols |
| 20 | `conventions/20-build-system.md` | CMake + scikit-build-core + presets + clang tooling |
| 21 | `interop/21-binding-strategy.md` | Why nanobind; module shape; GIL; NumPy bridge |
| 22 | `interop/22-exposed-api.md` | What crosses the boundary, in phased rollout |
| 23 | `interop/23-python-driver.md` | `cuny_sms` package, demo, analytics, notebooks |
| 24 | `interop/24-type-stubs-and-packaging.md` | `.pyi`, `py.typed`, wheel building |
| 25 | `25-todo-roadmap.md` | Phased build plan |
| 26 | `26-open-questions.md` | Deferred decisions |
