# 24 — Type Stubs and Packaging

The compiled `_core.so` looks like an opaque blob to `mypy` and IDEs unless we ship a stub file. The build also has to produce a wheel that can be `pip install`ed.

## Stubs (`.pyi`) — IDE & mypy support

nanobind ships `nanobind.stubgen`, a tool that introspects a compiled module and emits a `.pyi`:

```bash
python -m nanobind.stubgen \
    --module cuny_sms._core \
    --output-file python/cuny_sms/_core.pyi
```

Output looks like:

```python
# cuny_sms/_core.pyi (generated)
from __future__ import annotations
from enum import IntEnum
from typing import overload

class Department(IntEnum):
    MATH = 0
    CS = 1
    STATS = 2
    PHYS = 3

class Person:
    def __init__(self, first_name: str, last_name: str) -> None: ...
    @property
    def first_name(self) -> str: ...
    @property
    def last_name(self) -> str: ...

class PrerequisiteGraph:
    def __init__(self) -> None: ...
    def add_prereq(self, course: str, prereq: str) -> None: ...
    def can_enroll(self, student_id: int, course: str) -> bool: ...
    def transitive_closure(self, course: str) -> list[str]: ...
    def topological_order(self) -> list[str]: ...

class PrereqCycleError(Exception): ...
```

Run stubgen as a CMake post-build step (or an `entry_points` script) so stubs stay in sync.

## Wiring stubgen into CMake

```cmake
# bindings/CMakeLists.txt
add_custom_command(TARGET _core POST_BUILD
    COMMAND ${Python_EXECUTABLE} -m nanobind.stubgen
            --module cuny_sms._core
            --output-file ${CMAKE_SOURCE_DIR}/python/cuny_sms/_core.pyi
    BYPRODUCTS ${CMAKE_SOURCE_DIR}/python/cuny_sms/_core.pyi
    COMMENT "Generating Python stubs"
)
```

Now `cmake --build` rebuilds stubs whenever `_core` changes.

## Stub manifest — getting them in the wheel

`pyproject.toml`:

```toml
[tool.scikit-build]
wheel.packages = ["python/cuny_sms"]

# ensure .pyi shipped alongside .so
[tool.scikit-build.wheel]
include = ["python/cuny_sms/*.pyi"]
```

After `pip install`, the wheel layout is:

```
site-packages/cuny_sms/
    __init__.py
    _core.cpython-311-x86_64-linux-gnu.so
    _core.pyi          ← mypy and IDEs read this
    py.typed           ← marker file (PEP 561)
```

## `py.typed` marker — PEP 561 compliance

A zero-byte file that signals "this package has type info":

```
python/cuny_sms/py.typed
```

Without it, `mypy` ignores the package's type info even with stubs present.

## Wheel building

```bash
pip install -e .            # editable install for development
pip wheel . -w dist/        # produce a wheel for distribution
```

Resulting wheel: `cuny_sms-0.0.1-cp311-abi3-linux_x86_64.whl` (the `abi3` tag comes from nanobind's `STABLE_ABI` mode in `bindings/CMakeLists.txt`).

## Cross-platform later

`cibuildwheel` builds wheels for Linux, macOS, and Windows in CI. Out of scope for first build; flagged in `26-open-questions.md`.

## Verification

```bash
# typecheck the Python package against the stubs
mypy --strict python/

# round-trip import
python -c "from cuny_sms import PrerequisiteGraph; print(PrerequisiteGraph.__doc__)"

# stubs match runtime
python -c "
import cuny_sms._core as c
import inspect
sig = inspect.signature(c.PrerequisiteGraph.add_prereq)
assert list(sig.parameters) == ['self', 'course', 'prereq']
"
```

If `mypy --strict` passes and round-trip imports succeed, the wheel is good.

## Cross-references

- nanobind module shape — `21-binding-strategy.md`
- What gets exposed — `22-exposed-api.md`
- Build system overall — `conventions/20-build-system.md`
