"""Correctness oracle: kernel sqrt vs math.sqrt."""

# TODO: implement.
#
# import math, os
# import pytest
# from .bindings import kernel_sqrt_q32, kernel_sqrt_f64
#
# @pytest.mark.skipif(not os.path.exists("/dev/nm_newton"), reason="nm_newton not loaded")
# @pytest.mark.parametrize("x", [2.0, 100.0, 1e6, 1e10])
# def test_kernel_fp_matches_math(x):
#     assert math.isclose(kernel_sqrt_f64(x), math.sqrt(x), rel_tol=1e-12)
#
# @pytest.mark.skipif(not os.path.exists("/dev/nm_newton"), reason="nm_newton not loaded")
# @pytest.mark.parametrize("x", [2.0, 100.0, 1e6])
# def test_kernel_fixed_close_to_math(x):
#     # Convert to Q32.32, get sqrt back, convert out
#     x_q = int(x * (1 << 32))
#     r_q = kernel_sqrt_q32(x_q)
#     r   = r_q / (1 << 32)
#     assert math.isclose(r, math.sqrt(x), rel_tol=1e-6)  # Q32.32 quantization
