#include "linalg.h"
#include "todo.h"

namespace nm::linalg {

// TODO: QR via *modified* Gram-Schmidt (numerically more stable than the
// classical version).
//   for k in 0..n-1:
//     R(k,k) = ||A_col_k||
//     Q_col_k = A_col_k / R(k,k)
//     for j in k+1..n-1:
//       R(k,j) = Q_col_k . A_col_j
//       A_col_j -= R(k,j) * Q_col_k
// References: Wikipedia "QR_decomposition"; NR3 §2.10.
QR qr_decompose(const Matrix& /*A*/) { NM_TODO("linalg::qr_decompose"); }

}  // namespace nm::linalg
