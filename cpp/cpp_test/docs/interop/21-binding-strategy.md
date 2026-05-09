# 21 — Binding Strategy: nanobind

## Decision

**nanobind**, not pybind11. Reasons:

| Dimension | nanobind | pybind11 |
|-----------|----------|----------|
| Wheel size (typical) | ~4× smaller | larger |
| Compile time | ~2× faster | slower |
| NumPy interop | first-class via buffer protocol | works, but less direct |
| Required C++ standard | 17 minimum | 14 minimum |
| Stable ABI support | yes (`STABLE_ABI`) | partial |
| Ecosystem maturity | growing | very mature |

We're already on C++20 and our hottest cross-language hop is "attendance matrices as `numpy.ndarray`". nanobind wins on both counts. The trade-off is a smaller community — but the API mirrors pybind11's closely, so existing pybind11 docs translate trivially.

## Module shape

`bindings/_core.cpp` is the single translation unit that defines the Python module:

```cpp
#include <nanobind/nanobind.h>
#include <nanobind/stl/string.h>
#include <nanobind/stl/vector.h>
#include <nanobind/stl/optional.h>
#include <nanobind/ndarray.h>

#include <cuny_sms/identity.hpp>
#include <cuny_sms/role.hpp>
#include <cuny_sms/prerequisite_graph.hpp>
#include <cuny_sms/enrollment.hpp>
#include <cuny_sms/attendance.hpp>

namespace nb = nanobind;
using namespace cuny_sms;

NB_MODULE(_core, m) {
    m.doc() = "CUNY SMS native module";

    // Phase A — data classes
    nb::class_<Person>(m, "Person")
        .def(nb::init<std::string, std::string>(), nb::arg("first_name"), nb::arg("last_name"))
        .def_prop_ro("first_name", &Person::firstName)
        .def_prop_ro("last_name",  &Person::lastName);

    nb::class_<CunyMember>(m, "CunyMember")
        .def(nb::init<Person, CampusId, EmployeeId>())
        .def_prop_ro("email", &CunyMember::email)
        .def_prop_ro("employee_id", &CunyMember::employeeId);

    // Phase C — graph (release GIL on long calls)
    nb::class_<PrerequisiteGraph>(m, "PrerequisiteGraph")
        .def(nb::init<>())
        .def("add_prereq", &PrerequisiteGraph::addPrereq)
        .def("can_enroll", &PrerequisiteGraph::canEnroll,
             nb::call_guard<nb::gil_scoped_release>())
        .def("transitive_closure", &PrerequisiteGraph::transitiveClosure,
             nb::call_guard<nb::gil_scoped_release>())
        .def("topological_order", &PrerequisiteGraph::topologicalOrder,
             nb::call_guard<nb::gil_scoped_release>());

    // Exception translation
    nb::register_exception<PrereqCycleError>(m, "PrereqCycleError");
}
```

`NB_MODULE(_core, m)` is the macro that becomes the Python entry point. The compiled `.so` must be importable as `cuny_sms._core`, hence the package layout in `19-python-style.md`.

## Ownership and lifetimes

nanobind provides return-value policies analogous to pybind11's:

| Policy | When |
|--------|------|
| `nb::rv_policy::take_ownership` | Factory returns; Python owns and will free. |
| `nb::rv_policy::reference_internal` | Getter returns a reference into a Python-tracked parent; Python's parent keeps both alive. |
| `nb::rv_policy::copy` | Default for value types; safest. |
| `nb::rv_policy::move` | Move out of a temporary. |
| `nb::rv_policy::reference` | Raw reference; caller must guarantee lifetime. **Avoid.** |

Default to `copy` for value types (`Person`, `Course`, `AttendanceRecord`). For the long-lived service objects (`PrerequisiteGraph`, `EnrollmentService`), Python takes ownership of the C++ instance via `take_ownership` — they live as long as the Python wrapper does.

## GIL release for long operations

DFS over a 500-course catalog takes ~100 µs. That's short enough not to matter. But `transitiveClosure` of a large catalog, or a sweep-line conflict check across the whole department's schedule, can take milliseconds. Wrap those calls:

```cpp
.def("transitive_closure", &PrerequisiteGraph::transitiveClosure,
     nb::call_guard<nb::gil_scoped_release>())
```

This releases Python's Global Interpreter Lock for the duration of the C++ call. Other Python threads can run; on return, the GIL is reacquired. Critical for any real concurrent use.

## NumPy zero-copy via `nb::ndarray`

For the analytics functions that operate on attendance matrices, we want zero-copy:

```cpp
m.def("attendance_rate_per_student",
    [](nb::ndarray<double, nb::ndim<2>, nb::c_contig> mat) {
        // mat.shape(0) = students, mat.shape(1) = sessions
        // mat.data() points directly at NumPy's buffer
        std::vector<double> out(mat.shape(0));
        for (std::size_t i = 0; i < mat.shape(0); ++i) {
            double sum = 0.0;
            for (std::size_t j = 0; j < mat.shape(1); ++j)
                sum += mat(i, j);
            out[i] = sum / static_cast<double>(mat.shape(1));
        }
        return out;
    });
```

Caller in Python:

```python
import numpy as np
from cuny_sms._core import attendance_rate_per_student

mat = np.array([[1.0, 0.0, 1.0], [1.0, 1.0, 1.0]])
rates = attendance_rate_per_student(mat)
```

No copy; `mat.data()` points into NumPy's buffer.

## Exception translation

C++ exceptions don't cross language boundaries unscathed. nanobind translates standard exceptions automatically; for our custom ones (`PrereqCycleError`, `EnrollmentRejection`), register translators:

```cpp
nb::register_exception<PrereqCycleError>(m, "PrereqCycleError");
```

Python now sees `cuny_sms._core.PrereqCycleError` and can `except` it normally.

## Stub generation

nanobind ships `nanobind.stubgen`:

```bash
python -m nanobind.stubgen -m cuny_sms._core -o python/cuny_sms/_core.pyi
```

Runs once per build; produces a `.pyi` that `mypy` consumes. See `24-type-stubs-and-packaging.md`.

## What we *don't* expose

The C++ side has plenty that should stay C++-only:

- Internal data structures (LRU cache internals, hash bucket layouts, `WelfordAggregator`'s private state).
- The repository interface (`CunyDatabase`) — Python interacts via the service layer.
- Templates not instantiated for a known type — nanobind needs concrete types.

If something needs to be Python-visible later, we can always add the binding. The reverse is harder.

## Cross-references

- API exposure phases — `22-exposed-api.md`
- Type stubs and wheels — `24-type-stubs-and-packaging.md`
- Python-side wrapper — `23-python-driver.md`
