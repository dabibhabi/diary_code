"""B03 oracle: kernel Simpson result close to scipy."""

# TODO: implement.
#
# import os, math
# import pytest
# from .bindings import read_kernel_results, reference
#
# @pytest.fixture(scope="module")
# def results():
#     if not os.path.exists("/tmp/com1"):
#         pytest.skip("QEMU not running")
#     return read_kernel_results()
#
# def test_close_to_one_third(results):
#     ref = reference()
#     for n, val in results.items():
#         tol = max(1e-3 / n**2, 1e-7)  # Simpson is O(1/n^4); use loose envelope
#         assert math.isclose(val, ref, abs_tol=tol), f"n={n}: got {val}, expected {ref}"
#
# def test_error_decreases(results):
#     ref = reference()
#     errs = [(n, abs(val - ref)) for n, val in sorted(results.items())]
#     for (n1, e1), (n2, e2) in zip(errs, errs[1:]):
#         assert e2 <= e1, f"non-monotonic: n={n1}→{n2}, err {e1}→{e2}"
