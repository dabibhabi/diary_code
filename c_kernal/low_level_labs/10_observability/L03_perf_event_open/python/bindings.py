"""ctypes wrapper for the userspace saxpy reference.

Builds a small `.so` from `cpp/numerical_methods/c/blas1.c` so the notebook can
sweep N and time saxpy from Python while a separate `pmu_saxpy` process reads
the counters. For the simplest workflow, prefer parsing `pmu_saxpy`'s stdout.
"""

# TODO: build cpp/numerical_methods/c/blas1.c into a libnm_blas1.so
# (the parent CMake's NM_USE_PYTHON pybind11 module also exports these — pick one approach).
#
# Skeleton:
# import ctypes
# import numpy as np
# from pathlib import Path
#
# _LIB = ctypes.CDLL(str(Path(__file__).resolve().parents[4] / "build" / "libnm_blas1.so"))
# _LIB.nm_saxpy_f64.argtypes = [ctypes.c_size_t, ctypes.c_double,
#                                ctypes.POINTER(ctypes.c_double),
#                                ctypes.POINTER(ctypes.c_double)]
# _LIB.nm_saxpy_f64.restype = None
#
# def saxpy(alpha: float, x: np.ndarray, y: np.ndarray) -> None:
#     assert x.dtype == np.float64 and y.dtype == np.float64
#     assert x.shape == y.shape
#     _LIB.nm_saxpy_f64(x.size, alpha,
#                       x.ctypes.data_as(ctypes.POINTER(ctypes.c_double)),
#                       y.ctypes.data_as(ctypes.POINTER(ctypes.c_double)))
