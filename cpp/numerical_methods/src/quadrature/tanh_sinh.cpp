#include "quadrature.h"
#include "todo.h"

namespace nm::quadrature {

// TODO: Tanh-sinh (double exponential) quadrature.
//   Substitute x = tanh((π/2) sinh(t)); the integrand decays double-
//   exponentially at ±∞, so a uniform trapezoid in t converges fast even
//   when f has endpoint singularities.
// References: Wikipedia "Tanh-sinh_quadrature"; Bailey & Borwein.
double tanh_sinh(func /*f*/, double /*a*/, double /*b*/, double /*tol*/, int /*max_levels*/) {
    NM_TODO("quadrature::tanh_sinh");
}

}  // namespace nm::quadrature
