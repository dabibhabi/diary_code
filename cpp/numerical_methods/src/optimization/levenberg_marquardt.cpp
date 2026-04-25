#include "optimization.h"
#include "todo.h"

namespace nm::optimization {

// TODO: Levenberg-Marquardt nonlinear least-squares.
//   Repeat:
//     compute r = residuals(x), J = jacobian(x)
//     solve (J^T J + λ diag(J^T J)) Δx = -J^T r       // SPD system!
//     if cost decreases  → accept, λ /= 10  (toward Gauss-Newton)
//     else               → reject, λ *= 10  (toward gradient descent)
// LANGUAGE CHOICE: C++ for the host loop; the hot ops are J^T J and
//   solving the normal equations — link Cholesky from this library, or BLAS.
// DESIGN: this function depends on cholesky/lu_solve from linalg — good test
//   that your linear algebra primitives are usable as building blocks.
// QUANT USE: SABR/Heston/SVI vol-surface calibration.
OptResult levenberg_marquardt(ResidualFn /*r*/, JacobianFn /*J*/, linalg::Vector /*x0*/, double /*tol*/,
                              int /*max_iter*/) {
    NM_TODO("optimization::levenberg_marquardt");
}

}  // namespace nm::optimization
