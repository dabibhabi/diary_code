#include "linalg.h"
#include "todo.h"

namespace nm::linalg {

// TODO: Conjugate gradient (A symmetric positive-definite).
//   r0 = b - A x0;  p0 = r0
//   for k in 0..max_iter-1:
//     alpha = (r_k . r_k) / (p_k . A p_k)
//     x_{k+1} = x_k + alpha p_k
//     r_{k+1} = r_k - alpha A p_k
//     if ||r_{k+1}|| < tol → done
//     beta = (r_{k+1} . r_{k+1}) / (r_k . r_k)
//     p_{k+1} = r_{k+1} + beta p_k
// LANGUAGE CHOICE: C++ if A is dense and small. For huge sparse PDE matrices,
//   the matvec A*p dominates → port matvec to CUDA (cuSPARSE or hand kernel).
// References: Shewchuk's "Without the Agonizing Pain" tutorial.
Vector conjugate_gradient(const Matrix& /*A*/, const Vector& /*b*/, double /*tol*/, int /*max_iter*/) {
    NM_TODO("linalg::conjugate_gradient");
}

}  // namespace nm::linalg
