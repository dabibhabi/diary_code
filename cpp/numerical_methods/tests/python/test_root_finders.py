"""Python tests for the root-finding methods.

These talk to the C++ library through pybind11 (`nmpy`). The point is twofold:

1. Cross-language verification — same inputs through C++ and Python should
   give identical results to ~1e-10. If they don't, the C-Python adapter is
   wrong (lambda lifetime issue, ABI mismatch, etc.).

2. Stubbed methods raise `NotImplementedError` (mapped from C++
   `nm::not_implemented`). pytest is told to *xfail* those tests so CI
   stays green until you implement them.
"""
from __future__ import annotations

import math

import pytest

nmpy = pytest.importorskip("nmpy")


def _maybe_xfail(fn):
    """Catch NotImplementedError and turn it into an xfail."""
    def wrapper(*args, **kwargs):
        try:
            return fn(*args, **kwargs)
        except NotImplementedError as e:
            pytest.xfail(f"TODO: {e}")
    return wrapper


def test_bisection_sqrt2():
    r = nmpy.bisection(lambda x: x * x - 2.0, 0.0, 2.0, 1e-12)
    assert r.converged
    assert abs(r.root - math.sqrt(2.0)) < 1e-10


def test_newton_sqrt2():
    r = nmpy.newton(lambda x: x * x - 2.0, lambda x: 2.0 * x, 1.0, 1e-12)
    assert r.converged
    assert abs(r.root - math.sqrt(2.0)) < 1e-10
    # Empirical convergence order should be ~quadratic.
    p = nmpy.estimate_order(r.errors)
    assert p > 1.7, f"newton order ~ {p}, expected >1.7"


def test_brent_sqrt2():
    r = nmpy.brent(lambda x: x * x - 2.0, 0.0, 2.0, 1e-12)
    assert r.converged
    assert abs(r.root - math.sqrt(2.0)) < 1e-10


@_maybe_xfail
def test_halley_sqrt2():
    r = nmpy.halley(
        lambda x: x * x - 2.0,
        lambda x: 2.0 * x,
        lambda _: 2.0,
        1.0, 1e-12, 200,
    )
    assert r.converged
    assert abs(r.root - math.sqrt(2.0)) < 1e-10


@_maybe_xfail
def test_ridders_sqrt2():
    r = nmpy.ridders(lambda x: x * x - 2.0, 0.0, 2.0, 1e-12, 200)
    assert r.converged
    assert abs(r.root - math.sqrt(2.0)) < 1e-10


@_maybe_xfail
def test_chandrupatla_sqrt2():
    r = nmpy.chandrupatla(lambda x: x * x - 2.0, 0.0, 2.0, 1e-12, 200)
    assert r.converged
    assert abs(r.root - math.sqrt(2.0)) < 1e-10
