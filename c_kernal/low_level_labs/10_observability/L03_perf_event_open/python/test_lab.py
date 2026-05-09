"""pytest oracle for L03.

Goal: confirm the saxpy ctypes binding agrees with NumPy on randomized inputs.
This isn't testing the PMU reading — that's an end-to-end concern handled by
the `compare` Make target. This catches "did I link the right symbol".
"""

# TODO: implement once bindings.py is wired up.
#
# import numpy as np
# from .bindings import saxpy
#
# def test_saxpy_matches_numpy():
#     rng = np.random.default_rng(42)
#     x = rng.standard_normal(1024)
#     y_ours = rng.standard_normal(1024)
#     y_ref  = y_ours.copy()
#     alpha  = 0.5
#     saxpy(alpha, x, y_ours)
#     y_ref += alpha * x
#     np.testing.assert_allclose(y_ours, y_ref, rtol=1e-12)
