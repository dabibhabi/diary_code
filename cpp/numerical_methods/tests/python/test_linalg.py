"""Linalg cross-language tests."""
from __future__ import annotations

import pytest

nmpy = pytest.importorskip("nmpy")


def _maybe_xfail(fn):
    def wrapper(*args, **kwargs):
        try:
            return fn(*args, **kwargs)
        except NotImplementedError as e:
            pytest.xfail(f"TODO: {e}")
    return wrapper


def _make_spd_3x3():
    """Symmetric positive-definite 3x3 matrix."""
    M = nmpy.linalg.Matrix(3, 3)
    M[0, 0], M[0, 1], M[0, 2] = 4, 2, 1
    M[1, 0], M[1, 1], M[1, 2] = 2, 5, 3
    M[2, 0], M[2, 1], M[2, 2] = 1, 3, 6
    return M


@_maybe_xfail
def test_cholesky_factorization():
    A = _make_spd_3x3()
    L = nmpy.linalg.cholesky(A)
    # Verify L * L^T = A
    for i in range(3):
        for j in range(3):
            s = sum(L[i, k] * L[j, k] for k in range(3))
            assert abs(s - A[i, j]) < 1e-10


@_maybe_xfail
def test_conjugate_gradient_solves():
    A = _make_spd_3x3()
    b = [7.0, 10.0, 10.0]
    x = nmpy.linalg.conjugate_gradient(A, b, 1e-12, 100)
    # Verify A*x ≈ b
    for i in range(3):
        s = sum(A[i, j] * x[j] for j in range(3))
        assert abs(s - b[i]) < 1e-8
