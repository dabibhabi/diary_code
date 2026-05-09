# 23 — Python Driver

The Python side is a thin, ergonomic layer over the nanobind module. It is also where the math chapters become *runnable*.

## Package structure

```
python/cuny_sms/
    __init__.py            # re-exports public API from _core + helpers
    _core.pyi              # generated type stubs (see 24-)
    demo.py                # build a small Math/CS/Stats/Physics catalog and walk it
    catalog.py             # canned course list per department
    analytics.py           # WelfordAggregator-using analytics helpers
    notebooks/
        12-statistics.ipynb
        13-probability.ipynb
        14-information-theory.ipynb
        15-combinatorics.ipynb
        16-optimization.ipynb
        17-linear-algebra.ipynb
```

## `__init__.py` — single source of truth for the public API

```python
"""CUNY Student Management System — Python driver."""

from __future__ import annotations

from cuny_sms._core import (
    AttendanceService,
    Campus,
    Course,
    CunyMember,
    Department,
    EmailGenerator,
    EmployeeId,
    EnrollmentService,
    Person,
    PrerequisiteGraph,
    PrereqCycleError,
    Role,
    RoleType,
    WaitlistManager,
    WelfordAggregator,
)
from cuny_sms.analytics import attendance_rate, KL, mutual_information

__all__ = [
    "AttendanceService", "Campus", "Course", "CunyMember", "Department",
    "EmailGenerator", "EmployeeId", "EnrollmentService", "Person",
    "PrerequisiteGraph", "PrereqCycleError", "Role", "RoleType",
    "WaitlistManager", "WelfordAggregator",
    "attendance_rate", "KL", "mutual_information",
]
```

Importers see one flat namespace; nothing leaks `_core`.

## `catalog.py` — the toy curriculum

```python
from __future__ import annotations
from cuny_sms._core import Department


CANONICAL_COURSES: list[tuple[str, str, Department, int]] = [
    # (course_id, name, department, capacity)
    ("Math101", "Calculus I",          Department.MATH,  120),
    ("Math201", "Calculus II",         Department.MATH,  100),
    ("Math301", "Linear Algebra",      Department.MATH,   80),
    ("Math302", "Discrete Mathematics",Department.MATH,   80),
    ("CS101",   "Intro to CS",         Department.CS,    150),
    ("CS102",   "Data Structures",     Department.CS,    120),
    ("CS201",   "Algorithms",          Department.CS,    100),
    ("CS301",   "Systems Programming", Department.CS,     80),
    ("AI301",   "Intro to AI",         Department.CS,     60),
    ("AD401",   "Advanced AI",         Department.CS,     40),
    ("Stats101","Intro to Statistics", Department.STATS, 100),
    ("Stats201","Statistical Inference",Department.STATS, 80),
    ("Stats301","Bayesian Methods",    Department.STATS,  60),
    ("Phys101", "Mechanics",           Department.PHYS,  100),
    ("Phys201", "Electromagnetism",    Department.PHYS,   80),
    ("Phys301", "Quantum Mechanics",   Department.PHYS,   60),
]

CANONICAL_PREREQS: list[tuple[str, str]] = [
    ("Math201", "Math101"),
    ("Math301", "Math201"),
    ("CS102",   "CS101"),
    ("CS201",   "CS102"),
    ("CS201",   "Math101"),
    ("CS301",   "CS102"),
    ("CS301",   "CS201"),
    ("AI301",   "CS201"),
    ("AI301",   "Math201"),
    ("AD401",   "CS301"),
    ("AD401",   "Math301"),
    ("Stats201","Stats101"),
    ("Stats201","Math201"),
    ("Stats301","Stats201"),
    ("Phys201", "Phys101"),
    ("Phys201", "Math201"),
    ("Phys301", "Phys201"),
    ("Phys301", "Math301"),
]
```

A 16-course world that exercises every code path.

## `demo.py` — runnable walkthrough

```python
"""End-to-end smoke test using the canonical Math/CS/Stats/Physics catalog."""

from __future__ import annotations

from cuny_sms import (
    PrerequisiteGraph, PrereqCycleError,
    EnrollmentService, WaitlistManager,
    AttendanceService,
)
from cuny_sms.catalog import CANONICAL_COURSES, CANONICAL_PREREQS


def build_graph() -> PrerequisiteGraph:
    g = PrerequisiteGraph()
    for course, prereq in CANONICAL_PREREQS:
        g.add_prereq(course, prereq)
    return g


def main() -> None:
    g = build_graph()
    print("Topological order:", g.topological_order())
    print("Closure of AD401:", g.transitive_closure("AD401"))

    try:
        g.add_prereq("Math101", "AD401")  # would create a cycle
    except PrereqCycleError as e:
        print(f"Correctly rejected cycle: {e}")

    enroll = EnrollmentService(graph=g)
    waitlist = WaitlistManager()
    attendance = AttendanceService()
    # ... build a few members, enroll them, record attendance
    # (the bindings handle Member construction)


if __name__ == "__main__":
    main()
```

`python -m cuny_sms.demo` should print the topological order and prove the engine works end-to-end.

## `analytics.py` — math chapters made real

```python
"""Pure-Python helpers that wrap the C++ aggregators."""

from __future__ import annotations
from typing import Sequence
import numpy as np
from cuny_sms._core import WelfordAggregator


def attendance_rate(records: Sequence[bool]) -> tuple[float, float]:
    """Mean attendance and (sample) standard deviation, via Welford."""
    w = WelfordAggregator()
    for r in records:
        w.add(1.0 if r else 0.0)
    return w.mean(), np.sqrt(w.variance())


def KL(p: np.ndarray, q: np.ndarray, *, eps: float = 1e-12) -> float:
    """KL divergence D_KL(p || q) in bits, with Laplace smoothing."""
    p = np.asarray(p, dtype=np.float64)
    q = np.asarray(q, dtype=np.float64)
    p = p / p.sum()
    q = (q + eps) / (q.sum() + eps * len(q))
    mask = p > 0
    return float(np.sum(p[mask] * np.log2(p[mask] / q[mask])))


def mutual_information(joint: np.ndarray) -> float:
    """I(X;Y) = sum p(x,y) log2 (p(x,y) / (p(x) p(y)))."""
    joint = joint / joint.sum()
    px = joint.sum(axis=1, keepdims=True)
    py = joint.sum(axis=0, keepdims=True)
    expected = px @ py
    mask = joint > 0
    return float(np.sum(joint[mask] * np.log2(joint[mask] / expected[mask])))
```

Each math chapter (12–17) has a companion notebook that:

1. Imports `cuny_sms` + `numpy`.
2. Builds toy data (or pulls from `demo`'s state).
3. Uses the LaTeX from the chapter to label the formula being computed.
4. Runs the formula via `cuny_sms`'s C++ implementation and a NumPy reference, compares.

This makes the math chapters self-validating documentation.

## Jupyter usage

```bash
pip install -e ".[dev]"
jupyter lab python/cuny_sms/notebooks/
```

Each notebook is short — a few cells. Math chapter prose lives in markdown; this is the executable companion.

## Cross-references

- Module shape (`_core` + package) — `21-binding-strategy.md`
- API surface per phase — `22-exposed-api.md`
- Stubs and wheels — `24-type-stubs-and-packaging.md`
