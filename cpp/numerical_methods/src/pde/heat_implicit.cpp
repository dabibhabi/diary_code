#include "pde.h"
#include "todo.h"

namespace nm::pde {

// TODO: backward-Euler implicit scheme.
//   At each time step, solve a tridiagonal system:
//     -r u_{i-1}^{n+1} + (1+2r) u_i^{n+1} - r u_{i+1}^{n+1} = u_i^n
//   with boundary conditions absorbed into the RHS.
//   Use the Thomas algorithm for the tridiagonal solve (O(N)).
// LANGUAGE CHOICE: C/C++. Tridiagonal solves are not GPU-friendly per-step
//   (sequential by nature), so CPU wins unless you batch many independent
//   solves (one per asset, say) → cuSPARSE has gtsv2 for batched tridiag.
HeatGrid heat_implicit(double /*nu*/, double /*x_min*/, double /*x_max*/, double /*T*/, int /*N*/, int /*M*/,
                       InitialFn /*u0*/, BoundaryFn /*left*/, BoundaryFn /*right*/) {
    NM_TODO("pde::heat_implicit");
}

}  // namespace nm::pde
