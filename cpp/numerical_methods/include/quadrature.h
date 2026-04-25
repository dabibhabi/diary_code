#pragma once

#include "numerical_methods.h"

// Numerical integration. See TODO.md §2 for the full notes — what to read,
// why each one matters for quant work, and references.
namespace nm::quadrature {

// Romberg integration — repeated trapezoid + Richardson extrapolation.
//   max_levels: number of refinement levels (each doubles the points).
//   tol: stop when |T_{k,k} - T_{k-1,k-1}| < tol.
// References: Wikipedia "Romberg's_method"; NR3 §4.3.
double romberg(func f, double a, double b, int max_levels = 20, double tol = 1e-10);

// Adaptive Simpson — recursively bisect where Simpson(whole) and
// Simpson(left)+Simpson(right) disagree by more than tol.
// References: Wikipedia "Adaptive_Simpson's_method"; NR3 §4.7.
double adaptive_simpson(func f, double a, double b, double tol = 1e-10, int max_depth = 50);

// Gauss-Legendre quadrature — n-point rule integrates polynomials of
// degree <= 2n-1 exactly. Map [a,b] -> [-1,1] via t = (b-a)/2 * x + (a+b)/2.
//   Hint: precompute nodes & weights for n in {2,4,8,16}; or generate via
//         Newton on Legendre polynomials (NR3 §4.6).
double gauss_legendre(func f, double a, double b, int n);

// Gauss-Hermite quadrature — integrates ∫_{-∞}^{∞} e^{-x^2} g(x) dx.
// Quant relevance: any expectation under N(0,1) is a Gauss-Hermite integral.
//   For E[g(Z)] with Z ~ N(0,1), substitute x = z/sqrt(2):
//     E[g(Z)] = (1/sqrt(pi)) * sum_i w_i g(sqrt(2) x_i).
// References: Wikipedia "Gauss-Hermite_quadrature"; NR3 §4.6.4.
double gauss_hermite(func g, int n);

// Tanh-sinh (double exponential) — endpoint-singularity-tolerant.
// References: Wikipedia "Tanh-sinh_quadrature"; Bailey & Borwein.
double tanh_sinh(func f, double a, double b, double tol = 1e-12, int max_levels = 10);

}  // namespace nm::quadrature
