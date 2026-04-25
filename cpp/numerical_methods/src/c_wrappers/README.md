# `src/c_wrappers/` — C++ over the C API

Thin RAII layer that lets the C++ library call the pure-C primitives in
`c_api/` without exposing raw pointers to user code. The same C body runs
on CPU (in `c/`), in a `__device__` CUDA kernel, and via this wrapper.

## Cholesky wrapper (`cholesky_wrapper.cpp`)

For symmetric positive-definite $A \in \mathbb{R}^{n\times n}$:

$$
A = L\,L^{\!\top},\qquad L_{ii} = \sqrt{A_{ii} - \sum_{k<i} L_{ik}^2},\qquad
L_{ji} = \frac{A_{ji} - \sum_{k<i} L_{jk} L_{ik}}{L_{ii}}
$$

Throws `std::domain_error` if $A$ is not SPD; the C side reports this via a
return code and the wrapper translates to an exception (see
`docs/ERROR_HANDLING.md`).

### Modern C++ sketch (what you'll fill in)

```cpp
nm::linalg::Matrix nm::linalg::cholesky(const Matrix& A) {
    if (A.rows != A.cols)
        throw std::invalid_argument("cholesky: matrix must be square");

    Matrix L = A;                                  // copy: the C function works in place
    if (const int rc = nm_cholesky_inplace(L.data.data(), L.rows); rc != 0)
        throw std::domain_error("cholesky: matrix is not SPD");

    // Zero the strict upper triangle (the C function leaves it untouched).
    for (int i = 0; i < L.rows; ++i)
        for (int j = i + 1; j < L.cols; ++j)
            L(i, j) = 0.0;
    return L;
}
```

Notes on idiom:
- **`if (init; cond)`** — C++17 init-statement keeps `rc` scoped to the check.
- **`L.data.data()`** — the inner `data()` is `std::vector::data()`.
- **Copy then in-place** — pure-C function can't allocate; the C++ wrapper owns the copy.
- **Zero the upper triangle** — keeps tests like `for (j>=i) sum L(i,k)*L(j,k)` exact.

## Why this directory exists

A naive port would have `nm::linalg::cholesky` *call* the same algorithm
written in C++ idioms. We don't, because:

1. The CUDA kernel `cuda/cholesky_batch.cu` already calls
   `nm_cholesky_inplace` from `__device__` code. **One body, two backends.**
2. Pure C with `__restrict__` autovectorizes more reliably than `std::vector`
   plus iterators for hot loops.
3. The C primitive is callable from Rust, Go, ctypes Python — anything with
   a C FFI — without re-implementation.

The wrapper's sole job is RAII + exception translation at the boundary.
