#include "quadrature.h"
#include "todo.h"

namespace nm::quadrature {

// TODO: Adaptive Simpson.
//   Define S(a, b) = (b-a)/6 * (f(a) + 4 f((a+b)/2) + f(b)).
//   Recurse: if |S(a, m) + S(m, b) - S(a, b)| < 15 * tol  →  accept,
//   else split each half with tol/2. Cap recursion at max_depth.
// References: Wikipedia "Adaptive_Simpson's_method"; NR3 §4.7.
double adaptive_simpson(func /*f*/, double /*a*/, double /*b*/, double /*tol*/, int /*max_depth*/) {
    NM_TODO("quadrature::adaptive_simpson");
}

}  // namespace nm::quadrature
