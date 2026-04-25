#pragma once

// Compressed Sparse Row (CSR) matrix.
//
// Why this DSA fits CG:
//   - Conjugate gradient is FOR sparse SPD systems — that's the whole point.
//     Demonstrating CG on a dense matrix is academic; with a CSR matrix and
//     a sparse mat-vec, the speedup over a dense solver is real and visible.
//   - CSR also opens the door to a CUDA challenge: parallel sparse mat-vec
//     (cuSPARSE has csrmv; rolling your own is a common GPU programming
//     exercise).
//
// Format:
//   row_ptr.size() == n_rows + 1
//   col_idx.size() == values.size() == nnz
//   for row i: values in [row_ptr[i] .. row_ptr[i+1])
//              with column indices col_idx[same range]

#include <cstddef>
#include <vector>

namespace nm::dsa {

struct CSRMatrix {
    int                 rows = 0;
    int                 cols = 0;
    std::vector<int>    row_ptr;  // size rows+1
    std::vector<int>    col_idx;  // size nnz
    std::vector<double> values;   // size nnz

    std::size_t nnz() const { return values.size(); }
};

// y = A * x. Throws std::invalid_argument on size mismatch.
// TODO: implement.
std::vector<double> spmv(const CSRMatrix& A, const std::vector<double>& x);

// Build a 1-D Laplacian tridiagonal matrix in CSR form, useful for testing.
// (-1, 2, -1) on rows 0..n-1, dirichlet boundaries.
// TODO: implement.
CSRMatrix laplacian_1d(int n);

}  // namespace nm::dsa
