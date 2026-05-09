# 19 — Python Style & Conventions

The Python side is the driver: a thin wrapper over the C++ engine, plus analytics notebooks. Ergonomic, type-safe, and minimal.

## Style

- **PEP 8** with a 100-column limit.
- **Double quotes** by convention (matches `ruff format` default; cuts noise).
- **Type hints everywhere** — non-negotiable. `mypy --strict` runs in CI.
- **`__future__` annotations** at the top of every module: `from __future__ import annotations` so type hints are strings and don't pay runtime cost.

## Tooling

| Tool | Purpose | Config location |
|------|---------|-----------------|
| `ruff` | Lint + format | `pyproject.toml [tool.ruff]` |
| `mypy --strict` | Type checking | `pyproject.toml [tool.mypy]` |
| `pytest` | Tests | `pyproject.toml [tool.pytest.ini_options]` |
| `pytest-cov` | Coverage | invoked by pytest |
| `hypothesis` | Property tests on graph algorithms | imported in tests |
| `pre-commit` | Git hooks | `.pre-commit-config.yaml` |

Skeleton `pyproject.toml` for tooling (the build-system part lives in `20-build-system.md`):

```toml
[tool.ruff]
line-length = 100
target-version = "py311"

[tool.ruff.lint]
select = ["E", "F", "I", "N", "UP", "B", "A", "C4", "T20", "SIM", "RUF"]
ignore = ["E501"]  # ruff format handles line length

[tool.mypy]
python_version = "3.11"
strict = true
warn_unused_configs = true
disallow_untyped_decorators = true
plugins = []

[tool.pytest.ini_options]
addopts = "-ra --strict-markers --cov=cuny_sms --cov-report=term-missing"
testpaths = ["tests"]
```

---

## Project layout (Python side)

```
python/
  cuny_sms/
    __init__.py          # re-export from _core (the nanobind module)
    demo.py              # tiny Math/CS/Stats/Physics catalog + walkthrough
    analytics.py         # Welford, attendance rates wrapper
    notebooks/           # one .ipynb per math chapter (12-17)
  tests/
    test_identity.py
    test_prereq_graph.py
    test_enrollment.py
    test_attendance.py
    test_analytics.py
```

`_core` is the compiled nanobind module produced by the build system; `cuny_sms/__init__.py` re-exports its public API.

---

## Mirroring C++ types in Python

The boundary is narrow: nanobind exposes C++ classes, but Python-side code shouldn't *only* talk in those types. Wrap them in `dataclasses` where it improves ergonomics:

```python
from __future__ import annotations
from dataclasses import dataclass, field
from cuny_sms import _core


@dataclass(frozen=True, slots=True)
class Course:
    course_id: str
    name: str
    department: _core.Department
    capacity: int

    def to_native(self) -> _core.Course:
        return _core.Course(self.course_id, self.name, self.department, self.capacity)
```

`frozen=True, slots=True` gives immutability + memory efficiency. `to_native()` materializes the C++ form when needed.

---

## Protocols for cross-language interfaces

Use `typing.Protocol` to write Python that consumes either the C++ binding or a pure-Python mock:

```python
from __future__ import annotations
from typing import Protocol


class PrerequisiteGraph(Protocol):
    def add_prereq(self, course: str, prereq: str) -> None: ...
    def can_enroll(self, student_id: int, course: str) -> bool: ...
    def transitive_closure(self, course: str) -> list[str]: ...
```

Then `_core.PrerequisiteGraph` and a Python-side mock both satisfy `PrerequisiteGraph`. Tests use the mock; production uses the binding. `mypy` checks both paths.

---

## `Literal` for status enums

C++ enums become Python `IntEnum`s through nanobind. For the Python-side API, accepting `Literal["ENROLLED", "WAITLISTED", "DROPPED", "COMPLETED"]` is friendlier:

```python
from typing import Literal

EnrollmentStatus = Literal["ENROLLED", "WAITLISTED", "DROPPED", "COMPLETED"]


def filter_by_status(enrollments: list[Enrollment], status: EnrollmentStatus) -> list[Enrollment]:
    ...
```

Saves callers from importing the enum and gives `mypy` exhaustiveness checks.

---

## Property-based testing with `hypothesis`

The graph algorithms have invariants that should hold over arbitrary inputs:

- `addPrereq` followed by `removePrereq` must restore the graph.
- A topological order must respect every edge.
- Cycle detection must reject any graph containing a cycle and accept any DAG.

```python
from hypothesis import given, strategies as st
from cuny_sms import _core


@given(st.lists(st.tuples(st.text(min_size=2, max_size=5),
                          st.text(min_size=2, max_size=5)),
                max_size=20))
def test_topo_order_respects_edges(edges: list[tuple[str, str]]) -> None:
    g = _core.PrerequisiteGraph()
    for course, prereq in edges:
        if course == prereq:
            continue  # skip self-loops
        try:
            g.add_prereq(course, prereq)
        except _core.PrereqCycleError:
            return  # cycle is fine; we test the no-cycle case
    order = g.topological_order()
    position = {c: i for i, c in enumerate(order)}
    for course, prereq in edges:
        if course in position and prereq in position:
            assert position[prereq] < position[course]
```

This is far better than hand-picked test cases — hypothesis searches for minimal failing inputs automatically.

---

## Async — only at the I/O boundary

Python's `asyncio` is great for I/O multiplexing (HTTP, DB), useless for CPU work. Pattern:

```python
import asyncio
from cuny_sms import _core


async def fetch_and_record(client, member_id: int, attendance: AttendanceService) -> None:
    record = await client.fetch_record(member_id)   # I/O bound, async
    attendance.record(record)                       # CPU bound, sync, into C++
```

Never `asyncio.run` a graph traversal. Run it sync; the C++ binding releases the GIL for long calls (see `interop/21-binding-strategy.md`).

---

## Avoid

- `from x import *` — implicit imports break tooling.
- Mutable default arguments (`def f(x=[])`).
- Catching `Exception` without re-raising or narrowing.
- Polluting `__init__.py` with logic — it should mostly be re-exports.
- Logging via `print` in library code — use `logging` with named loggers.

---

## Cross-references

- `pyproject.toml` build sections — `20-build-system.md`
- nanobind binding details — `interop/21-binding-strategy.md`
- Type stubs (`.pyi`) for the C++ module — `interop/24-type-stubs-and-packaging.md`
