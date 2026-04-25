#include "pde.h"
#include "todo.h"

namespace nm::pde {

// TODO: Crank-Nicolson (average of explicit + implicit).
//   -r/2 u_{i-1}^{n+1} + (1+r) u_i^{n+1} - r/2 u_{i+1}^{n+1}
//     =  r/2 u_{i-1}^n   + (1-r) u_i^n   + r/2 u_{i+1}^n
//   O(dt^2) accurate, unconditionally stable, but oscillates near
//   non-smooth payoffs. Pair with Rannacher start (do 2 fully-implicit
//   steps with dt/2 first, then switch to CN).
// QUANT USE: standard for European/American option PDE pricing — once
//   working, change the operator to LV/Heston and you have a real pricer.
HeatGrid crank_nicolson(double /*nu*/, double /*x_min*/, double /*x_max*/, double /*T*/, int /*N*/, int /*M*/,
                        InitialFn /*u0*/, BoundaryFn /*left*/, BoundaryFn /*right*/) {
    NM_TODO("pde::crank_nicolson");
}

}  // namespace nm::pde
