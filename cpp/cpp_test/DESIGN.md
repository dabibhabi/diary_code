# CUNY Student Management System — Design

A multi-campus student management engine: identity, roles, prerequisite-aware enrollment, attendance tracking, and the analytics that fall out. **C++17/20 core**, **Python** driver via **nanobind**.

For a build-status snapshot and quick start, see [`README.md`](README.md). This file is the design index.

## Status snapshot (also in [README.md](README.md) and [docs/25-todo-roadmap.md](docs/25-todo-roadmap.md))

| Phase | | What |
|-------|:--:|------|
| 0 | ✅ | Scaffolding |
| 1 | ✅ | Identity model |
| 2 | ✅ | Abstract Factory (4 split staff factories + Course/Section + `CampusServices` facade) |
| 3 | ✅ | `PrerequisiteGraph` (DFS / Kahn / Tarjan / LRU memo / AND-OR / BFS) |
| 4 | ⬜ | Enrollment + Conflict Strategy + Waitlist |
| 5 | ⬜ | Attendance + dual-index + Welford |
| 6 | ⬜ | Repository (in-memory) |
| 7 | ⬜ | nanobind bindings |
| 8 | ⬜ | Python driver + analytics notebooks |
| 9 | ⬜ | Hardening |

## Where to read

- **First time here?** Start with [`docs/01-overview.md`](docs/01-overview.md), then [`docs/02-architecture.md`](docs/02-architecture.md).
- **Implementing?** Follow [`docs/25-todo-roadmap.md`](docs/25-todo-roadmap.md) phase by phase.
- **Browsing topics?** Full TOC at [`docs/00-index.md`](docs/00-index.md).
- **Building / testing?** [`README.md`](README.md) for `make` targets; [`docs/conventions/20-build-system.md`](docs/conventions/20-build-system.md) for the CMake details.

## Pillars

| Concern | Chapter |
|---------|---------|
| Architecture & data model | [`docs/02-architecture.md`](docs/02-architecture.md) |
| Identity, roles | [`docs/03-domain-model.md`](docs/03-domain-model.md) |
| Patterns (Factory, Strategy, Repository) | [`docs/04-patterns.md`](docs/04-patterns.md) |
| Runtime flows | [`docs/05-flows.md`](docs/05-flows.md) |
| Algorithms | [`docs/algorithms/`](docs/algorithms/) (06–11) |
| Math foundations | [`docs/math/`](docs/math/) (12–17) |
| Code conventions | [`docs/conventions/`](docs/conventions/) (18–20) |
| C++/Python interop | [`docs/interop/`](docs/interop/) (21–24) |
| Roadmap | [`docs/25-todo-roadmap.md`](docs/25-todo-roadmap.md) |
| Open questions | [`docs/26-open-questions.md`](docs/26-open-questions.md) |

## Scope (first build)

- **Course catalog**: Math, CS, Statistics, Physics only.
- **Persistence**: in-memory; SQLite deferred.
- **Bindings**: nanobind.
- **Math depth**: applied formulas with LaTeX in mermaid; runnable as Jupyter notebooks under `python/cuny_sms/notebooks/`.

## Project layout

See [`README.md`](README.md) for the full annotated directory tree. Headline:

- `include/cuny_sms/{domain, factories, graph, persistence, strategies}/` — headers organized by design pattern.
- `data/catalogs/` — 12 TSV files with 452 real CUNY courses scraped from CCNY/Brooklyn/Hunter.
- `data/test_names/` — 500 first + 500 last names for unit-test fixtures.
- `tests/` — Catch2 v3 suite, 37 cases, all passing under ASan + UBSan + `-Werror`.
- `docs/` — 28 design chapters.
