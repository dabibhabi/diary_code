#include "linalg.h"
#include "todo.h"

namespace nm::linalg {

// TODO: LU decomposition with partial pivoting.
//   For each column k:
//     find row p with max |A[i,k]| (i >= k); swap rows p and k; record piv[k]=p.
//     for i in k+1..n-1:  A[i,k] /= A[k,k];  A[i, k+1:] -= A[i,k] * A[k, k+1:]
//   Pull L (unit lower) and U (upper) out of the in-place result.
// LANGUAGE CHOICE: C++ for clarity. For huge dense systems, link BLAS
//   (dgetrf) instead — never reimplement what MKL/OpenBLAS does in 5 ns/flop.
// DESIGN PATTERN: returning a struct with the factorization and a separate
//   solve() function is the "Strategy" + "Stateful Object" combo — same
//   factorization can serve many right-hand sides cheaply.
// References: Wikipedia "LU_decomposition"; NR3 §2.3.
LU lu_decompose(const Matrix& /*A*/) { NM_TODO("linalg::lu_decompose"); }

// TODO: forward + back substitution given the LU factorization.
Vector lu_solve(const LU& /*factor*/, const Vector& /*b*/) { NM_TODO("linalg::lu_solve"); }

}  // namespace nm::linalg
