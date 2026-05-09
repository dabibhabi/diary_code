# CUNY Student Management System

Multi-campus student management engine for CUNY (CCNY, Brooklyn College, Hunter College). C++17/20 core, Python driver via nanobind. Math, CS, Statistics, Physics catalogs only (first build).

For the design rationale see [`DESIGN.md`](DESIGN.md). For the phased build plan and current progress see [`docs/25-todo-roadmap.md`](docs/25-todo-roadmap.md).

---

## Status at a glance

| Phase | Status | What |
|-------|:--:|------|
| 0 | ✅ | Scaffolding (CMake, presets, clang-format/tidy, pre-commit, pyproject.toml) |
| 1 | ✅ | Identity model — `Name`, `Person`, `Email`, `EmployeeId`, `Campus`, `CunyMember` |
| 2 | ✅ | Abstract Factory — split into `StudentFactory` / `FacultyFactory` / `TeachingAssistantFactory` / `AdminFactory` / `CourseFactory` / `SectionFactory` + `CampusServices` facade |
| 3 | ✅ | `PrerequisiteGraph` — DFS cycle detection, Kahn's topo sort, Tarjan's SCC, transitive closure with LRU cache, AND/OR group satisfaction, BFS shortest path |
| 4 | ⬜ | `EnrollmentService` + `ConflictDetector` Strategy + `WaitlistManager` |
| 5 | ⬜ | `AttendanceService` + dual-index hash maps + Welford streaming aggregator |
| 6 | ⬜ | `CunyDatabase` Repository interface + `InMemoryCunyDatabase` |
| 7 | ⬜ | nanobind Python bindings |
| 8 | ⬜ | Python driver + math-chapter notebooks |
| 9 | ⬜ | Hardening (fuzzing, benchmarks, hypothesis property tests) |

**Side-quests done outside the roadmap:**

| | What | Where |
|---|---|---|
| ✅ | 12 real CUNY course-catalog TSV files (452 courses) | `data/catalogs/` |
| ✅ | 1000 test names (500 first + 500 last, NYC-diverse) | `data/test_names/` |
| ✅ | Header reorg into `domain/` / `factories/` / `graph/` / `persistence/` / `strategies/` | `include/cuny_sms/` |
| ✅ | `struct` vs `class` convention adopted | see *Conventions* below |

---

## Quick start

```bash
make test            # configure with sanitizers, build, run all 37 tests
make run             # build and run the C++ demo
make help            # full list of targets
```

Requirements: CMake ≥ 3.20, Ninja, a C++20 compiler (GCC ≥ 11, Clang ≥ 14, MSVC ≥ 19.30).

---

## Project layout

```
.
├── README.md                       ← you are here
├── DESIGN.md                       ← design index → docs/
├── Makefile                        ← convenience over CMake
├── CMakeLists.txt
├── CMakePresets.json
├── pyproject.toml                  ← scikit-build-core (Phase 7)
├── main.cpp                        ← C++ demo binary
│
├── include/cuny_sms/
│   ├── identity.hpp                ← umbrella header
│   ├── domain/                     ← composite-component VALUE TYPES (struct keyword)
│   │   ├── name.hpp                struct Name
│   │   ├── person.hpp              struct Person
│   │   ├── email.hpp               struct Email
│   │   ├── employee_id.hpp         struct EmployeeId
│   │   ├── course_id.hpp           struct CourseId
│   │   ├── course.hpp              struct Course
│   │   ├── campus.hpp              struct Campus
│   │   ├── campus_code.hpp         enum CampusCode
│   │   ├── department.hpp          enum Department  (MATH | CS | STATS | PHYS)
│   │   ├── admin_level.hpp         enum AdminLevel  (IT | REGISTRAR | DEPT_CHAIR | DEAN | PROVOST)
│   │   ├── meeting_slot.hpp        struct DaySet, struct MeetingSlot
│   │   ├── section.hpp             struct Section
│   │   ├── role.hpp                class Role + StudentRole/ProfessorRole/TeachingAssistantRole/AdminRole
│   │   └── cuny_member.hpp         class CunyMember  ← composes all of the above
│   ├── factories/                  ← creational patterns (Factory, Abstract Factory, Facade)
│   │   ├── factory_support.hpp
│   │   ├── student_factory.hpp     class StudentFactory + Ccny/Brooklyn/Hunter
│   │   ├── faculty_factory.hpp     class FacultyFactory + Ccny/Brooklyn/Hunter
│   │   ├── teaching_assistant_factory.hpp
│   │   ├── admin_factory.hpp       class AdminFactory + Ccny/Brooklyn/Hunter
│   │   ├── course_factory.hpp      class CourseFactory + Ccny/Brooklyn/Hunter
│   │   ├── section_factory.hpp     class SectionFactory + Ccny/Brooklyn/Hunter
│   │   └── campus_services.hpp     class CampusServices  ← abstract-factory facade
│   ├── graph/
│   │   └── prerequisite_graph.hpp  class PrerequisiteGraph
│   ├── persistence/
│   │   └── catalog_loader.hpp      class CatalogLoader
│   └── strategies/                 ← Phase 4 placeholder (ConflictDetector strategies)
│
├── src/                            ← implementation files (header-only for now)
├── tests/                          ← Catch2 v3 unit tests
│   ├── test_identity.cpp           18 cases
│   ├── test_prerequisite_graph.cpp 15 cases
│   ├── test_catalog_loader.cpp     3 cases
│   └── test_smoke.cpp              1 case
├── bindings/                       ← nanobind stub (Phase 7)
├── python/cuny_sms/                ← Python package skeleton (Phase 7)
│
├── data/
│   ├── catalogs/                   ← 12 TSV files, 452 real CUNY courses
│   └── test_names/                 ← 500 first + 500 last names
│
└── docs/                           ← 28 design chapters (see DESIGN.md)
```

---

## Conventions

### `struct` vs `class`

- **`struct`** — small composite-component value types used as building blocks. Public-by-default keyword *as documentation*. Examples: `Name`, `Email`, `Person`, `Campus`, `Course`, `Section`, `MeetingSlot`.
- **`class`** — larger objects that compose structs and own behavior or invariants. Examples: `CunyMember`, every factory, `CampusServices`, `PrerequisiteGraph`, `CatalogLoader`, `Role` and its polymorphic subclasses.

When you see `struct X`, expect it to be a piece of data destined to be composed into a bigger `class`. When you see `class Y`, expect behavior, invariants, and probably composition of structs.

### Pattern → directory

| Directory | What lives here |
|-----------|-----------------|
| `domain/` | Value types and enums — the *building blocks* |
| `factories/` | Creational patterns — Factory, Abstract Factory, Facade |
| `graph/` | Graph data structures (DAG, etc.) |
| `persistence/` | Loaders and (later) repositories |
| `strategies/` | Behavioral pattern — Strategy (Phase 4 onward) |

Find a class by its design pattern, not by its alphabetical position.

### Coding style

- C++20, `target_compile_features(... cxx_std_20)`.
- `-Wall -Wextra -Wpedantic -Wshadow -Wconversion -Wsign-conversion -Werror`.
- ASan + UBSan active in the dev preset.
- `camelBack` methods, `CamelCase` types, `kPrefix` for `constexpr` constants, `_` suffix for private members.
- Header-only by default; switch to STATIC + .cpp split if a translation unit grows large.

Full conventions: [`docs/conventions/18-cpp-style.md`](docs/conventions/18-cpp-style.md).

---

## What's tested

37 unit tests (`make test`):

| File | Cases | Covers |
|------|---:|--------|
| `test_identity.cpp` | 18 | `Name`/`Person`/`Email`/`EmployeeId`/`Campus`/`CunyMember`, all four staff factories, `CourseFactory`, `SectionFactory`, `CampusServices` |
| `test_prerequisite_graph.cpp` | 15 | cycle detection, topo order, Tarjan's SCC, transitive closure, AND/OR satisfaction, BFS shortest path, cache invalidation |
| `test_catalog_loader.cpp` | 3 | path convention, loads all 12 TSV files (≥ 400 courses total), error paths |
| `test_smoke.cpp` | 1 | scaffolding sanity |

All pass under ASan + UBSan + `-Werror`.

---

## What's next

Pick a phase and tell the assistant. Reasonable next steps in order:

1. **Phase 4** — `EnrollmentService` + Strategy-based `ConflictDetector` + FIFO `WaitlistManager`. Adds `strategies/` directory's first occupants. Cross-references `docs/05-flows.md`, `docs/algorithms/08-schedule-conflict.md`, `docs/algorithms/09-waitlist-queues.md`.
2. **Phase 5** — `AttendanceService` with dual-index hash maps and Welford streaming aggregator. Cross-references `docs/algorithms/07-attendance-indexing.md`, `docs/algorithms/10-streaming-aggregation.md`, `docs/math/12-statistics.md`.
3. **Wire the catalog data** — turn the 452 loaded courses into a runtime `Catalog` object indexed by `CourseId`. Useful before Phase 4 since enrollment needs course lookup.

Or sweep the side-quest backlog (clang-tidy lints, more property tests, fill in `strategies/` README placeholder).
