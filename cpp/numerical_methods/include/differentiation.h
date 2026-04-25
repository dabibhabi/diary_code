#pragma once

#include "numerical_methods.h"

#include <complex>
#include <functional>

// Derivative methods that go beyond plain central difference. See TODO.md §3.
namespace nm::differentiation {

// Richardson extrapolation on central differences.
//   Compute D(h), D(h/2), ..., D(h/2^levels); kill the leading O(h^2) terms
//   via Richardson to reach O(h^{2*levels}) accuracy.
// References: Wikipedia "Richardson_extrapolation"; NR3 §5.7.
func richardson_derivative(func f, double h0 = 0.1, int levels = 4);

// Complex-step derivative — no subtractive cancellation.
//   f'(x) ~= Im( f(x + i h) ) / h    (h ~ 1e-20 is fine)
//   Requires f to accept complex inputs (templated or std::complex<double>).
// References: Squire & Trapp 1998; Wikipedia "Numerical_differentiation".
double complex_step_derivative(std::function<std::complex<double>(std::complex<double>)> f, double x, double h = 1e-20);

}  // namespace nm::differentiation
