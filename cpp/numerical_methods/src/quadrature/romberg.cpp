#include "quadrature.h"
#include "todo.h"

namespace nm::quadrature {

// TODO: Romberg integration.
//   T(k, 0) = composite trapezoid with 2^k subintervals (reuse previous
//             evaluations — only sum the new midpoints).
//   T(k, j) = (4^j * T(k, j-1) - T(k-1, j-1)) / (4^j - 1)         (Richardson)
//   Stop when |T(k,k) - T(k-1,k-1)| < tol.
// References: Wikipedia "Romberg's_method"; NR3 §4.3.
double romberg(func /*f*/, double /*a*/, double /*b*/, int /*max_levels*/, double /*tol*/) {
    NM_TODO("quadrature::romberg");
}

}  // namespace nm::quadrature
