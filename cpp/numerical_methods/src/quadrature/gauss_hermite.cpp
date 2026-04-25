#include "quadrature.h"
#include "todo.h"

namespace nm::quadrature {

// TODO: Gauss-Hermite quadrature.
//   Returns ∫_{-∞}^{∞} e^{-x^2} g(x) dx ≈ sum_i w_i g(x_i).
//   For E[g(Z)] under Z ~ N(0,1):
//     E[g(Z)] = (1/√π) sum_i w_i g(√2 * x_i).
// References: Wikipedia "Gauss-Hermite_quadrature"; NR3 §4.6.4.
double gauss_hermite(func /*g*/, int /*n*/) { NM_TODO("quadrature::gauss_hermite"); }

}  // namespace nm::quadrature
