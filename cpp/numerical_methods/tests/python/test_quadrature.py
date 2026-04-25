"""Cross-language quadrature checks. See test_root_finders.py for the
xfail-on-stubbed-method pattern."""
from __future__ import annotations

import math

import pytest

nmpy = pytest.importorskip("nmpy")


def _maybe_xfail(fn):
    def wrapper(*args, **kwargs):
        try:
            return fn(*args, **kwargs)
        except NotImplementedError as e:
            pytest.xfail(f"TODO: {e}")
    return wrapper


def test_simpson_x_squared():
    """The existing Simpson integrator (utils::integrate) already works."""
    r = nmpy.integrate(lambda x: x * x, 0.0, 1.0, 1000)
    assert abs(r - 1.0 / 3.0) < 1e-9


@_maybe_xfail
def test_romberg_sin():
    r = nmpy.quadrature.romberg(math.sin, 0.0, math.pi, 12, 1e-12)
    assert abs(r - 2.0) < 1e-10


@_maybe_xfail
def test_gauss_legendre_polynomial_exact():
    # 8-point Gauss-Legendre is exact for polynomials of degree <= 15.
    r = nmpy.quadrature.gauss_legendre(lambda x: x ** 6, 0.0, 1.0, 8)
    assert abs(r - 1.0 / 7.0) < 1e-12


@_maybe_xfail
def test_gauss_hermite_normal_second_moment():
    # E[Z^2] under N(0,1) = 1.
    # Substitute x = z/sqrt(2): E[g(Z)] = (1/sqrt(pi)) * sum w_i g(sqrt(2) x_i).
    # Easiest to test by integrating e^{-x^2}*x^2 directly = sqrt(pi)/2.
    r = nmpy.quadrature.gauss_hermite(lambda x: x * x, 16)
    assert abs(r - math.sqrt(math.pi) / 2.0) < 1e-8
