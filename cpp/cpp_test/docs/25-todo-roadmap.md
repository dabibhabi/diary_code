# 25 — TODO / Phased Roadmap

Concrete, checkable build order. Tick boxes as you go. Cross-references to per-chapter detail are in each phase.

**Live status as of latest commit:**

| Phase | | What | Tests |
|-------|:--:|------|------:|
| 0 | ✅ | Scaffolding | smoke 1 |
| 1 | ✅ | Identity model | identity 18 |
| 2 | ✅ | Abstract Factory (4 split staff + Course + Section + CampusServices facade) | identity (above) |
| 3 | ✅ | PrerequisiteGraph | graph 15 |
| — | ✅ | Catalog data scraped (12 files, 452 courses) | loader 3 |
| — | ✅ | Test name fixtures (500+500) | — |
| — | ✅ | Header reorg into pattern-named directories | — |
| 4 | ⬜ | EnrollmentService + Conflict Strategy + WaitlistManager | — |
| 5 | ⬜ | AttendanceService + dual-index + Welford | — |
| 6 | ⬜ | Repository (in-memory) | — |
| 7 | ⬜ | nanobind bindings | — |
| 8 | ⬜ | Python driver + analytics notebooks | — |
| 9 | ⬜ | Hardening | — |

Total: **37 / 37** unit-test cases passing under ASan + UBSan + `-Werror`.

---

## ✅ Phase 0 — Scaffolding (DONE)

- [x] Directory tree per `conventions/20-build-system.md` (`include/`, `src/`, `tests/`, `python/`, `bindings/`).
- [x] `CMakeLists.txt` (top-level) with `cuny_sms_core` library.
- [x] `CMakePresets.json` with `dev` (Debug + ASan + UBSan), `release`, `tsan`, `python` presets.
- [x] `.clang-format`, `.clang-tidy`, `.gitignore`, `.pre-commit-config.yaml`.
- [x] Smoke test wired with Catch2 v3 via `FetchContent`.
- [x] Top-level `Makefile` driving the presets (`make build`, `make dev`, `make test`, `make run`, `make tidy`, `make format`, `make clean`).

---

## ✅ Phase 1 — Identity model (DONE+)

Originally specified, all complete. **Extras shipped beyond the original scope** are noted.

- [x] `Name` as its own `struct` (split from Person — extra modularity).
- [x] `Person` composes `Name` + optional DOB.
- [x] `EmployeeId` with `std::mt19937` (no `std::rand`) — value cached at construction.
- [x] `Email` validated wrapper + `EmailGenerator` deterministic, campus-aware (3-digit suffix per `math/13-probability.md`).
- [x] `Department` enum (`MATH | CS | STATS | PHYS`).
- [x] `AdminLevel` enum (`IT | REGISTRAR | DEPT_CHAIR | DEAN | PROVOST`).
- [x] `Campus` + `CampusCode` value types.
- [x] `CunyMember` composes Person/Campus/EmployeeId/Email/`List<Role>`; move-only.
- [x] **(extra)** `Role` polymorphic hierarchy: `StudentRole`, `ProfessorRole`, `TeachingAssistantRole`, `AdminRole` with `clone()`. Phase-2-territory but landed here.
- [x] **(extra)** `Course`, `CourseId`, `Section`, `MeetingSlot`, `DaySet`. Originally Phase 4 territory.

---

## ✅ Phase 2 — Roles + Abstract Factory (DONE+)

Originally one `RoleFactory` per campus. Final: **4 separate factory hierarchies + Course + Section + a campus-level facade**.

- [x] `Role` hierarchy with `AdminLevel` field (in Phase 1 above).
- [x] `StudentFactory` abstract + `Ccny/Brooklyn/Hunter` concrete (`admit()`).
- [x] `FacultyFactory` abstract + 3 concrete (`hire()`) — Brooklyn requires non-empty specialization.
- [x] `TeachingAssistantFactory` abstract + 3 concrete (`assign()`) — Brooklyn requires non-empty supervisor.
- [x] `AdminFactory` abstract + 3 concrete (`appoint()`) — Hunter requires dept scope for IT/REGISTRAR/DEPT_CHAIR.
- [x] `CourseFactory` abstract + 3 concrete (`createCourse()`) — campus-specific course-id format (CCNY `MATH 20100`, Brooklyn `MATH 2001`, Hunter `MATH 201`).
- [x] `SectionFactory` abstract + 3 concrete (`createSection()`) — campus-specific section format (CCNY `01`, Brooklyn `TY1`, Hunter `01H`).
- [x] `CampusServices::forCampus(code)` — facade that returns a coherent family of factories.
- [x] Tests: every factory's per-campus rule exercised; `CampusServices` end-to-end.

---

## ✅ Phase 3 — Prerequisite graph (DONE)

- [x] `PrerequisiteGraph` with `addCourse`, `addPrereq`, `removePrereq`, `transitiveClosure`, `topologicalOrder`, `findCycles`, `shortestPath`, `isSatisfied`, `courseCount`.
- [x] Speculative directed-reachability cycle check on `addPrereq` (throws `PrereqCycleError`).
- [x] Iterative DFS for transitive closure (stack-safe).
- [x] **LRU cache** for closures (capacity 256), invalidated on `addPrereq`/`removePrereq`/`addCourse`.
- [x] **Tarjan's SCC** for `findCycles()` — returns SCCs of size ≥ 2.
- [x] **Kahn's BFS** topological sort with secondary cycle defense.
- [x] **BFS layering** for `shortestPath()`.
- [x] **AND/OR group satisfaction** for `isSatisfied()` — AND across distinct group keys + ungrouped, OR within a group key.
- [x] `std::hash<CourseId>` specialization in `namespace std`.
- [x] 15 test cases / 37 assertions on the canonical Math/CS/Stats/Physics example DAG.

---

## ⬜ Phase 4 — Enrollment + Conflict + Waitlist

> Goal: `enroll(member, course)` runs the full pipeline.

Cross-refs: [`docs/05-flows.md`](05-flows.md), [`docs/algorithms/08-schedule-conflict.md`](algorithms/08-schedule-conflict.md), [`docs/algorithms/09-waitlist-queues.md`](algorithms/09-waitlist-queues.md).

- [ ] `ConflictDetector` interface in `include/cuny_sms/strategies/` (the directory's first occupant — see `conventions/20-build-system.md`).
- [ ] `TimeOverlapStrategy` (pairwise via existing `Section::conflictsWith`).
- [ ] `RoomDoubleBookingStrategy` and `RoleAwareStrategy`.
- [ ] `CompositeConflictStrategy` (Composite over Strategy).
- [ ] `EnrollmentService::enroll` orchestrating prereq → conflict → capacity → waitlist.
- [ ] `WaitlistManager` with FIFO `std::deque` — `addToWaitlist`, `promoteFromWaitlist`.
- [ ] `EnrollmentResult` as `std::variant<Enrolled, Waitlisted, Rejected>`.
- [ ] Tests for every branch of the flow + waitlist promotion atomicity.

**Success criterion:** `tests/test_enrollment.cpp` passes including waitlist promotion on drop.

---

## ⬜ Phase 5 — Attendance + streaming aggregators

Cross-refs: [`docs/algorithms/07-attendance-indexing.md`](algorithms/07-attendance-indexing.md), [`docs/algorithms/10-streaming-aggregation.md`](algorithms/10-streaming-aggregation.md), [`docs/math/12-statistics.md`](math/12-statistics.md).

- [ ] `AttendanceRecord` packed struct (~64 bytes; cache-line friendly).
- [ ] `AttendanceService` with `byCourseDateIndex` + `byStudentDateIndex` (read-write-locked).
- [ ] `record(...)` updates both indexes synchronously.
- [ ] `WelfordAggregator` per course + parallel-merge form (Chan).
- [ ] `CountMinSketch` for top-k absentees (optional polish).
- [ ] Async persist hook (no-op until Phase 6 wires the DB).
- [ ] Tests: dual-index consistency; Welford correctness vs a NumPy reference; throughput sanity.

---

## ⬜ Phase 6 — Repository (in-memory only)

- [ ] `CunyDatabase` interface in `include/cuny_sms/persistence/`.
- [ ] `InMemoryCunyDatabase` implementing every method via `std::unordered_map` / `std::vector`.
- [ ] Refactor services to take `CunyDatabase&` instead of holding state directly.
- [ ] Tests: services compose with the in-memory DB; round-trip member + enrollment + attendance.

SQLite + Postgres impls explicitly deferred (see [`26-open-questions.md`](26-open-questions.md)).

---

## ⬜ Phase 7 — nanobind bindings

Cross-refs: [`docs/interop/21-binding-strategy.md`](interop/21-binding-strategy.md), [`docs/interop/22-exposed-api.md`](interop/22-exposed-api.md).

- [ ] Populate `bindings/_core.cpp` (`NB_MODULE(_core, m)`).
- [ ] Phase A — data classes (Name/Person/CunyMember).
- [ ] Phase B — pure functions (`EmailGenerator::generate`).
- [ ] Phase C — `PrerequisiteGraph` with `gil_scoped_release` on traversals.
- [ ] Phase D — services (Enrollment, Waitlist).
- [ ] Phase E — `AttendanceService` + `WelfordAggregator` + NumPy bridge via `nb::ndarray`.
- [ ] Set up `pyproject.toml` build (already drafted; verify with `pip install -e .`).
- [ ] Generate `_core.pyi` stubs via `nanobind.stubgen` post-build.

---

## ⬜ Phase 8 — Python driver + analytics notebooks

- [ ] `cuny_sms.catalog` module (load TSV files at runtime).
- [ ] `cuny_sms.demo` end-to-end script.
- [ ] `cuny_sms.analytics` (KL divergence, mutual info, attendance_rate).
- [ ] One Jupyter notebook per math chapter (12–17), each validating the C++ implementation against a NumPy reference.

---

## ⬜ Phase 9 — Hardening

- [ ] Hypothesis property tests on `PrerequisiteGraph` (random graph mutations preserve invariants).
- [ ] Microbenchmarks (Catch2 `BENCHMARK` or Google Benchmark) for `transitiveClosure`, `record`, sweep-line conflict.
- [ ] TSan run of the attendance service under concurrent writes.
- [ ] Cross-check that every diagram in `docs/` references a real symbol.

---

## Side-quests (separate from the phased roadmap)

| | Done | Notes |
|---|---|---|
| ✅ | Scrape real CCNY/Brooklyn/Hunter course catalogs | 452 courses in 12 TSV files (`data/catalogs/`); CCNY CS + Physics noted PARTIAL (JS-rendered source pages) |
| ✅ | Generate test-name fixtures | 500 first + 500 last names, NYC-diverse (`data/test_names/`) |
| ✅ | Header reorg into pattern-named directories | `domain/` / `factories/` / `graph/` / `persistence/` / `strategies/` |
| ✅ | `struct` vs `class` keyword convention | structs for composite components, classes for big composers — see [`conventions/18-cpp-style.md`](conventions/18-cpp-style.md) |
| ⬜ | Address aspirational clang-tidy lints | `readability-container-contains`, `modernize-use-designated-initializers`, `modernize-use-ranges` — style nits, build is currently clean without them |
| ⬜ | Wire scraped catalog into a runtime `Catalog` | turn the 452 loaded `CatalogEntry` rows into a `Catalog` object indexed by `(CampusCode, CourseId)`. Useful before Phase 4 |
| ⬜ | Backfill missing CCNY catalog rows | the partial CCNY CS (22 rows) + Physics (11 rows) need a JS-aware scrape or manual entry to be complete |
| ⬜ | Update `docs/03-domain-model.md` and `docs/04-patterns.md` to mention the split factories | text refresh; current diagrams still reference the old monolithic StaffFactory |

---

## Crosscutting — done at every phase

- Sanitizers (ASan + UBSan) clean.
- `clang-tidy` clean for the **error-graded** checks (aspirational lints noted above).
- `clang-format` clean.
- New code has tests; coverage ≥ 80%.
- Tests added under `tests/test_<phase>.cpp`; line in `tests/CMakeLists.txt` updated.

---

## Cross-references

- Per-phase technical detail — the chapter referenced in each phase row.
- Open questions parking lot — [`26-open-questions.md`](26-open-questions.md).
- Build/test entrypoints — top-level [`README.md`](../README.md) and [`Makefile`](../Makefile).
