#pragma once

#include <functional>
#include <vector>

// 1-D parabolic PDE solvers (heat equation u_t = nu * u_xx).
// QUANT relevance: Black-Scholes reduces to the heat equation under a change
// of variables, so this is the foundation of grid-based option pricing.
// See TODO.md §9.
namespace nm::pde {

// Initial condition u(x, 0).
using InitialFn = std::function<double(double)>;

// Boundary values u(x_min, t) and u(x_max, t).
using BoundaryFn = std::function<double(double /*t*/)>;

struct HeatGrid {
    std::vector<double> x;  // size N+1
    std::vector<double> u;  // size N+1, final state at t = T
};

// Forward-time / centered-space (FTCS): explicit. Stable iff
// nu * dt / dx^2 <= 1/2.
// References: Wikipedia "Finite_difference_method"; NR3 §20.2.
HeatGrid heat_explicit(double nu, double x_min, double x_max, double T, int N, int M, InitialFn u0, BoundaryFn left,
                       BoundaryFn right);

// Backward Euler — fully implicit. Unconditionally stable, O(dt) accurate.
// Solve a tridiagonal system per time step.
HeatGrid heat_implicit(double nu, double x_min, double x_max, double T, int N, int M, InitialFn u0, BoundaryFn left,
                       BoundaryFn right);

// Crank-Nicolson — average of explicit + implicit. O(dt^2), unconditionally
// stable. Note: oscillates near non-smooth payoffs — pair with Rannacher
// start (a couple of fully-implicit steps first).
// References: Wikipedia "Crank-Nicolson_method"; Wilmott ch. 77-80.
HeatGrid crank_nicolson(double nu, double x_min, double x_max, double T, int N, int M, InitialFn u0, BoundaryFn left,
                        BoundaryFn right);

}  // namespace nm::pde
