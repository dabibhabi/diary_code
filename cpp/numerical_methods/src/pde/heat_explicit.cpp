#include "pde.h"
#include "todo.h"

namespace nm::pde {

// TODO: explicit FTCS scheme for u_t = ν u_xx.
//   dx = (x_max - x_min) / N;  dt = T / M;
//   r = ν * dt / dx^2.   STABILITY: r <= 1/2 (otherwise solution blows up).
//   Update: u_new[i] = u[i] + r * (u[i+1] - 2 u[i] + u[i-1]).
// LANGUAGE CHOICE: each grid point at step n+1 only depends on its
//   neighbors at step n → embarrassingly parallel → CUDA wins for large N.
//   See cuda/pde_heat_explicit.cu for the CUDA challenge.
HeatGrid heat_explicit(double /*nu*/, double /*x_min*/, double /*x_max*/, double /*T*/, int /*N*/, int /*M*/,
                       InitialFn /*u0*/, BoundaryFn /*left*/, BoundaryFn /*right*/) {
    NM_TODO("pde::heat_explicit");
}

}  // namespace nm::pde
