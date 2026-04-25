#pragma once

#include <stdexcept>
#include <vector>

// Minimal dense linear algebra. See TODO.md §5.
namespace nm::linalg {

using Vector = std::vector<double>;

struct Matrix {
    int                 rows = 0;
    int                 cols = 0;
    std::vector<double> data;

    Matrix() = default;
    Matrix(int r, int c) : rows(r), cols(c), data(static_cast<size_t>(r) * c, 0.0) {}

    double&       operator()(int i, int j) { return data[i * cols + j]; }
    const double& operator()(int i, int j) const { return data[i * cols + j]; }
};

// LU decomposition with partial pivoting: P A = L U.
// L is unit lower-triangular, U is upper-triangular, piv stores the row swaps.
// References: Wikipedia "LU_decomposition"; NR3 §2.3.
struct LU {
    Matrix           L;
    Matrix           U;
    std::vector<int> piv;
};
LU     lu_decompose(const Matrix& A);
Vector lu_solve(const LU& factor, const Vector& b);

// Cholesky decomposition: A = L L^T for symmetric positive-definite A.
// Throws std::domain_error if A is not SPD.
// QUANT: used to draw correlated normals — Y = L Z gives Cov(Y) = A.
// References: Wikipedia "Cholesky_decomposition"; NR3 §2.9; Glasserman §2.3.
Matrix cholesky(const Matrix& A);

// QR via modified Gram-Schmidt. Q is m x n, R is n x n upper-triangular.
// References: Wikipedia "QR_decomposition"; NR3 §2.10.
struct QR {
    Matrix Q;
    Matrix R;
};
QR qr_decompose(const Matrix& A);

// Conjugate gradient — iterative solver for symmetric positive-definite A.
// References: Wikipedia "Conjugate_gradient_method"; Shewchuk's tutorial.
Vector conjugate_gradient(const Matrix& A, const Vector& b, double tol = 1e-10, int max_iter = 1000);

}  // namespace nm::linalg
