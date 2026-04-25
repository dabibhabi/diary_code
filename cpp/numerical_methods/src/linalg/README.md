# `src/linalg/` — dense linear algebra

Cholesky lives in `src/c_wrappers/cholesky_wrapper.cpp` (it's a wrapper around
the shared C primitive). Everything else lives here.

The shared types are in `include/linalg.h`:

```cpp
using Vector = std::vector<double>;

struct Matrix {
    int                 rows = 0, cols = 0;
    std::vector<double> data;                  // row-major
    double&       operator()(int i, int j)       { return data[i*cols + j]; }
    const double& operator()(int i, int j) const { return data[i*cols + j]; }
};
```

## LU with partial pivoting (`lu.cpp`)

Factor $P\,A = L\,U$, then solve $A\,x = b$ via two triangular solves:

$$
L\,y = P\,b,\qquad U\,x = y
$$

The factorization is $\Theta(n^3)$; each subsequent solve is $\Theta(n^2)$ —
**that's the whole point of factor-then-solve.**

### Modern C++17 sketch (Doolittle, partial pivot, in-place)

```cpp
LU lu_decompose(const Matrix& A) {
    if (A.rows != A.cols) throw std::invalid_argument("lu: square only");
    const int n = A.rows;
    Matrix    LU = A;                          // overwritten in place
    std::vector<int> piv(n);
    std::iota(piv.begin(), piv.end(), 0);      // 0, 1, 2, ..., n-1

    for (int k = 0; k < n; ++k) {
        // Find pivot row
        int    p     = k;
        double pmax  = std::abs(LU(k, k));
        for (int i = k + 1; i < n; ++i)
            if (const double v = std::abs(LU(i, k)); v > pmax) { p = i; pmax = v; }
        if (p != k) {
            for (int j = 0; j < n; ++j) std::swap(LU(k, j), LU(p, j));
            std::swap(piv[k], piv[p]);
        }
        if (LU(k, k) == 0.0) throw std::domain_error("lu: singular");

        for (int i = k + 1; i < n; ++i) {
            LU(i, k) /= LU(k, k);                         // store L below diag
            for (int j = k + 1; j < n; ++j)
                LU(i, j) -= LU(i, k) * LU(k, j);          // update U
        }
    }
    // Split LU into separate L (unit lower) and U (upper).
    Matrix L(n, n), U(n, n);
    for (int i = 0; i < n; ++i) {
        L(i, i) = 1.0;
        for (int j = 0; j < i; ++j)  L(i, j) = LU(i, j);
        for (int j = i; j < n; ++j)  U(i, j) = LU(i, j);
    }
    return {std::move(L), std::move(U), std::move(piv)};
}
```

Notes:
- **`std::iota`** initializes the pivot table.
- **`if (init; cond)`** — C++17 init-statement keeps `v` scoped.
- **Aggregate brace return** — `LU` struct is constructed in place.

## QR via modified Gram-Schmidt (`qr.cpp`)

For $A = Q\,R$ with $Q$ orthonormal:

$$
\text{for }k = 0,\ldots,n-1:\quad
R_{kk} = \|a_k\|_2,\quad q_k = a_k / R_{kk},\quad
R_{kj} = q_k^{\!\top} a_j,\quad a_j \mathrel{-}= R_{kj}\,q_k
$$

The **modified** variant is numerically more stable than the classical one
(re-orthogonalization happens incrementally instead of after the fact).

## Conjugate gradient (`conjugate_gradient.cpp`)

For $A$ symmetric positive-definite, iterates:

$$
\begin{aligned}
\alpha_k &= \frac{r_k^{\!\top} r_k}{p_k^{\!\top} A p_k}, & x_{k+1} &= x_k + \alpha_k p_k, \\
r_{k+1} &= r_k - \alpha_k A p_k, & \beta_k &= \frac{r_{k+1}^{\!\top} r_{k+1}}{r_k^{\!\top} r_k}, \\
p_{k+1} &= r_{k+1} + \beta_k p_k. &
\end{aligned}
$$

The hot operation is the matvec $A p_k$. For dense $A$ it's $O(n^2)$; pair
with `dsa::CSRMatrix` to get $O(\text{nnz})$ — the whole point of CG.

```cpp
Vector conjugate_gradient(const Matrix& A, const Vector& b,
                          double tol, int max_iter) {
    const int n = b.size();
    Vector x(n, 0.0), r = b, p = r;            // x0 = 0
    double rr = std::inner_product(r.begin(), r.end(), r.begin(), 0.0);
    for (int k = 0; k < max_iter; ++k) {
        Vector Ap = matvec(A, p);              // hot path → use SpMV when sparse
        const double pAp   = std::inner_product(p.begin(), p.end(), Ap.begin(), 0.0);
        const double alpha = rr / pAp;
        for (int i = 0; i < n; ++i) { x[i] += alpha * p[i]; r[i] -= alpha * Ap[i]; }
        const double rr_new = std::inner_product(r.begin(), r.end(), r.begin(), 0.0);
        if (std::sqrt(rr_new) < tol) return x;
        const double beta = rr_new / rr;
        for (int i = 0; i < n; ++i) p[i] = r[i] + beta * p[i];
        rr = rr_new;
    }
    return x;                                   // didn't converge; caller checks
}
```
