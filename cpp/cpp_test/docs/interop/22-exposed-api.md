# 22 — Exposed API

What crosses the C++/Python boundary, in the order it gets exposed. Each phase below is a distinct vertical slice — exposing more of the C++ engine to Python — and maps to a phase in the roadmap (`25-todo-roadmap.md`).

## Phase A — Identity (data classes)

The simplest types. Pure value objects, no behavior beyond accessors.

| C++ type | Python name | Notes |
|----------|-------------|-------|
| `Person` | `cuny_sms.Person` | `frozen` value object |
| `Campus` | `cuny_sms.Campus` | Identifies institution |
| `EmployeeId` | `cuny_sms.EmployeeId` | 9-digit int wrapper |
| `Email` | `cuny_sms.Email` | Validated string wrapper |
| `CunyMember` | `cuny_sms.CunyMember` | Composes all of the above |
| `Department` | `cuny_sms.Department` | enum: `MATH | CS | STATS | PHYS` |

Bindings are mechanical — accessors only. Property tests verify that a roundtrip Python → C++ → Python is identity.

## Phase B — Pure functions

Stateless utilities first. No object lifetime concerns.

| Function | Signature | Notes |
|----------|-----------|-------|
| `EmailGenerator.generate` | `(campus, person, employee_id) -> Email` | The format-by-rule function |
| `validate_course_id` | `(str) -> bool` | Deterministic, easy to test |

## Phase C — Prerequisite graph

The first stateful object Python touches. Heavy computational hot-path candidate; release GIL on traversals.

```python
from cuny_sms import PrerequisiteGraph, PrereqCycleError

g = PrerequisiteGraph()
g.add_prereq("CS102", "CS101")
g.add_prereq("CS201", "CS102")

try:
    g.add_prereq("CS101", "CS201")  # cycle!
except PrereqCycleError as e:
    print(f"rejected: {e}")

g.transitive_closure("CS201")     # ["CS101", "CS102"]
g.topological_order()             # ["CS101", "CS102", "CS201"]
g.can_enroll(student_id=42, course="CS201")
```

**Methods exposed.**

- `add_prereq(course: str, prereq: str) -> None` — raises `PrereqCycleError`
- `remove_prereq(course: str, prereq: str) -> None`
- `can_enroll(student_id: int, course: str) -> bool` — needs DB plug-in
- `transitive_closure(course: str) -> list[str]`
- `topological_order() -> list[str]`
- `find_cycles() -> list[list[str]]` — Tarjan's SCC

## Phase D — Services

The orchestration layer. These methods take + return composite types from Phase A.

| Service | Methods |
|---------|---------|
| `EnrollmentService` | `enroll(member, course) -> EnrollmentResult` |
| `WaitlistManager` | `add(member, course) -> int` (position), `promote(course) -> Optional[Member]` |
| `RoleService` | `add_role(member, role_type, role_data)`, `remove_role(member, role)` |

`EnrollmentResult` is a tagged union (C++ `std::variant`, Python sum type via nanobind). Possible variants:

- `Enrolled(enrollment)`
- `Waitlisted(position)`
- `Rejected(reason: str, missing_prereqs: list[str], conflicting_course: Optional[str])`

The Python side switches on the variant; type-checker gets exhaustiveness.

## Phase E — Attendance + analytics

The last and richest phase. Includes both the recording API and the streaming aggregators.

```python
from cuny_sms import AttendanceService
import numpy as np

svc = AttendanceService()
svc.record(enrollment_id=1, course_id="CS101", student_id=42,
           date="2026-05-09", status="PRESENT", minutes=60)

# query patterns
roster = svc.get_class_attendance("CS101", "2026-05-09")
day    = svc.get_student_attendance(42, "2026-05-09")

# streaming aggregators directly accessible
welford = svc.welford_for_course("CS101")
welford.mean(), welford.variance()

# zero-copy NumPy bridge for analytics
mat = svc.attendance_matrix("CS101")  # returns np.ndarray
rates = mat.mean(axis=1)              # one rate per student
```

**Methods exposed.**

- `record(...)`
- `get_class_attendance(course, date) -> list[AttendanceRecord]`
- `get_student_attendance(student, date) -> list[AttendanceRecord]`
- `welford_for_course(course) -> WelfordAggregator`
- `attendance_matrix(course) -> np.ndarray` — zero-copy via `nb::ndarray`
- `top_k_absentees(k: int) -> list[tuple[int, int]]` — backed by count-min sketch

The aggregators (`WelfordAggregator`, `CountMinSketch`, etc. from `algorithms/10-streaming-aggregation.md`) are themselves bound; analytics notebooks can use them directly without constructing an `AttendanceService`.

## What's *not* exposed

| C++ thing | Why not |
|-----------|---------|
| `LruCache<K, V>` | Internal optimization; users see closures, not cache lines |
| `HashIndexBucket` | Internal layout |
| `CunyDatabase` interface | Python uses services, services use the DB |
| Sanitizer hooks, internal asserts | Debug-only |
| Concept declarations (`RoleFactoryLike` etc.) | Pure type-system tooling |

## Verification

For each binding phase, three tests:

1. **Roundtrip identity** — construct from Python, read back, compare equal.
2. **Property test** (hypothesis) — random sequences of operations preserve invariants.
3. **GIL release** — call a long operation from one Python thread while another does compute; both make progress.

## Cross-references

- Why nanobind specifically — `21-binding-strategy.md`
- The `cuny_sms` Python package shape — `23-python-driver.md`
- Stubs and packaging — `24-type-stubs-and-packaging.md`
