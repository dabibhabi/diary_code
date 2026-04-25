#include "dsa/csr.h"

#include "todo.h"

namespace nm::dsa {

// TODO — sparse matvec:
//   for i in 0..rows:
//     y[i] = sum over j in [row_ptr[i], row_ptr[i+1])  values[j] * x[col_idx[j]]
std::vector<double> spmv(const CSRMatrix& /*A*/, const std::vector<double>& /*x*/) { NM_TODO("dsa::spmv"); }

// TODO — build the 1-D Laplacian:
//   row 0:    diag 2, super -1
//   row i:    sub -1, diag 2, super -1   (1 <= i < n-1)
//   row n-1:  sub -1, diag 2
CSRMatrix laplacian_1d(int /*n*/) { NM_TODO("dsa::laplacian_1d"); }

}  // namespace nm::dsa
