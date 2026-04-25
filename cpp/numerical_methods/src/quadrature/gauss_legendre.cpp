#include "quadrature.h"
#include "todo.h"

namespace nm::quadrature {

// TODO: Gauss-Legendre quadrature.
//   1. Get nodes x_i in [-1, 1] and weights w_i for the n-point rule.
//      Either tabulate (n in {2,4,8,16}) or compute via Newton's method on
//      Legendre polynomials (NR3 §4.6 has a clean routine).
//   2. Map to [a, b]:  t_i = (b-a)/2 * x_i + (a+b)/2.
//   3. Return (b-a)/2 * sum_i w_i f(t_i).
// References: Wikipedia "Gauss-Legendre_quadrature"; NR3 §4.6.
double gauss_legendre(func /*f*/, double /*a*/, double /*b*/, int /*n*/) { NM_TODO("quadrature::gauss_legendre"); }

}  // namespace nm::quadrature
