// C++ wrapper around c_api/cholesky_inplace.h.
//
// The point: the library's C++ API (`nm::linalg::cholesky`) calls the C
// primitive — same code that runs in the CUDA kernel.

#include "c_api/cholesky_inplace.h"
#include "linalg.h"
#include "todo.h"

#include <stdexcept>

namespace nm::linalg {

// TODO — once nm_cholesky_inplace is implemented, finish the wrapper:
//   1. Validate A is square: if (A.rows != A.cols) throw std::invalid_argument(...);
//   2. Make a copy: Matrix L = A;
//   3. Call: int rc = nm_cholesky_inplace(L.data.data(), L.rows);
//   4. if (rc != 0) throw std::domain_error("matrix is not SPD");
//   5. Zero the strict upper triangle of L.
//   6. return L.
Matrix cholesky(const Matrix& /*A*/) { NM_TODO("linalg::cholesky"); }

}  // namespace nm::linalg
