#pragma once

#include <functional>
#include <vector>

// Ordinary differential equations. See TODO.md §4.
// State vector y is std::vector<double>; RHS f(t, y) returns dy/dt.
namespace nm::ode {

using State = std::vector<double>;
using RHS   = std::function<State(double, const State&)>;

struct Trajectory {
    std::vector<double> t;
    std::vector<State>  y;
};

// Forward Euler — explicit, conditionally stable. Local error O(h^2).
//   y_{n+1} = y_n + h f(t_n, y_n)
// References: Wikipedia "Euler_method"; NR3 §17.0–17.1.
Trajectory euler(RHS f, double t0, State y0, double t_end, double h);

// Classical Runge-Kutta 4 — 4 stages, local error O(h^5). The default
// non-stiff workhorse before adaptive methods.
// References: Wikipedia "Runge-Kutta_methods"; NR3 §17.1.
Trajectory rk4(RHS f, double t0, State y0, double t_end, double h);

// Dormand-Prince adaptive RK45 — embedded 4(5) pair; uses local error
// estimate to grow/shrink the step. This is what MATLAB's ode45 implements.
//   tol: per-step relative+absolute tolerance.
// References: Wikipedia "Dormand-Prince_method"; NR3 §17.2.
Trajectory rk45(RHS f, double t0, State y0, double t_end, double h_init = 1e-3, double tol = 1e-8);

}  // namespace nm::ode
