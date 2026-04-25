#pragma once

#include "linalg.h"
#include "numerical_methods.h"

#include <functional>

// Function minimization. See TODO.md §6.
namespace nm::optimization {

struct OptResult {
    linalg::Vector x;
    double         f_value    = 0.0;
    int            iterations = 0;
    bool           converged  = false;
};

// Golden-section search — 1D, derivative-free, bracket-based. Each step
// shrinks the bracket by the golden ratio. Reference: NR3 §10.2.
double golden_section(func f, double a, double b, double tol = 1e-8);

// Brent's 1D minimizer — golden-section + parabolic interpolation. NR3 §10.3.
double brent_min(func f, double a, double b, double tol = 1e-8, int max_iter = 100);

// Nelder-Mead simplex — n-D derivative-free. Maintains an n+1-vertex
// simplex; reflect / expand / contract / shrink toward lower f-values.
// References: Wikipedia "Nelder-Mead_method"; NR3 §10.5.
using ObjectiveND = std::function<double(const linalg::Vector&)>;
OptResult nelder_mead(ObjectiveND f, linalg::Vector x0, double tol = 1e-8, int max_iter = 1000);

// Levenberg-Marquardt — nonlinear least-squares. Minimizes
// 0.5 * sum_i r_i(x)^2 by interpolating between Gauss-Newton (small lambda)
// and gradient descent (large lambda).
// QUANT: SABR/Heston/SVI vol-surface calibration is exactly this shape.
// References: Wikipedia "Levenberg-Marquardt_algorithm"; NR3 §15.5.
using ResidualFn = std::function<linalg::Vector(const linalg::Vector&)>;
using JacobianFn = std::function<linalg::Matrix(const linalg::Vector&)>;
OptResult levenberg_marquardt(ResidualFn r, JacobianFn J, linalg::Vector x0, double tol = 1e-8, int max_iter = 200);

}  // namespace nm::optimization
