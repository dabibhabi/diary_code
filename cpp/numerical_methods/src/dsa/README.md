# `src/dsa/` — data structures and algorithms

Targeted DSA additions where they materially improve a numerical method.

## Min-heap by error (`error_heap.cpp`)

Used by adaptive Simpson to process the highest-error subinterval first.
Heap invariant on a max-heap by `error_est`:

$$
\forall i:\quad \text{data}[i].\mathrm{error\_est} \;\geq\; \text{data}[2i+1].\mathrm{error\_est},\;\text{data}[2i+2].\mathrm{error\_est}
$$

`pop_max` is $O(\log n)$, vs. $O(n)$ for a linear scan. With a heap-based
schedule, adaptive quadrature halts the moment $\max\text{error} < \epsilon$.

### Modern C++17 sketch (sift-down core)

```cpp
void ErrorHeap::sift_down(std::size_t i) {
    const std::size_t n = data_.size();
    while (true) {
        const std::size_t l = 2 * i + 1, r = 2 * i + 2;
        std::size_t       best = i;
        if (l < n && data_[l].error_est > data_[best].error_est) best = l;
        if (r < n && data_[r].error_est > data_[best].error_est) best = r;
        if (best == i) break;
        std::swap(data_[i], data_[best]);
        i = best;
    }
}
```

You could equivalently use `std::priority_queue` with a custom comparator.
Rolling our own here is teaching value — and `peek_max` (read top without
popping) needs custom code anyway.

## Strongly-typed tridiagonal (`tridiag_struct.cpp`)

Wraps the three diagonals as one type:

$$
T = \begin{bmatrix} b_0 & c_0 & & \\ a_1 & b_1 & c_1 & \\ & \ddots & \ddots & \ddots \\ & & a_{n-1} & b_{n-1} \end{bmatrix}
$$

```cpp
std::vector<double> nm::dsa::solve(const Tridiag& T, const std::vector<double>& rhs) {
    if (!T.valid() || T.size() != rhs.size())
        throw std::invalid_argument("solve: size mismatch");

    auto a = T.sub, b = T.diag, c = T.super, d = rhs;   // mutable copies
    std::vector<double> x(rhs.size());
    if (nm_thomas_solve(rhs.size(), a.data(), b.data(), c.data(),
                        d.data(), x.data()) != 0)
        throw std::domain_error("tridiagonal: zero pivot");
    return x;
}
```

## Compressed Sparse Row (`csr.cpp`)

For row $i$, the values are stored at indices `[row_ptr[i], row_ptr[i+1])`,
with column indices in `col_idx`. SpMV:

$$
y_i \;=\; \sum_{j \,=\, \mathrm{row\_ptr}[i]}^{\mathrm{row\_ptr}[i+1]-1}
        \mathrm{values}[j]\,\cdot\,x[\,\mathrm{col\_idx}[j]\,]
$$

```cpp
std::vector<double> spmv(const CSRMatrix& A, const std::vector<double>& x) {
    if (static_cast<int>(x.size()) != A.cols)
        throw std::invalid_argument("spmv: dimension mismatch");
    std::vector<double> y(A.rows, 0.0);
    for (int i = 0; i < A.rows; ++i) {
        double s = 0.0;
        for (int j = A.row_ptr[i]; j < A.row_ptr[i + 1]; ++j)
            s += A.values[j] * x[A.col_idx[j]];
        y[i] = s;
    }
    return y;
}
```

Pair this with conjugate gradient (`linalg::conjugate_gradient`) to make CG
demonstrably useful — the Laplacian on $n=10^6$ is sparse with $\le 3$
non-zeros per row, so SpMV cost is $\Theta(n)$ instead of $\Theta(n^2)$.

## FFT — Cooley-Tukey radix-2 (stretch, `fft.cpp`)

DFT:

$$
X_k \;=\; \sum_{n=0}^{N-1} x_n \, e^{-2\pi i\, kn/N}
$$

Cooley-Tukey factors this as two half-size DFTs:

$$
X_k = X^{\text{even}}_k + e^{-2\pi i\, k/N}\,X^{\text{odd}}_k
$$

After bit-reversal permutation, the algorithm is $\log_2 N$ butterfly passes
of $N/2$ multiply-adds each, total $O(N \log N)$.

```cpp
// Sketch — one butterfly stage:
for (int s = 1; s <= log_n; ++s) {
    const int m = 1 << s;
    const std::complex<double> wm = std::polar(1.0, -2.0 * M_PI / m);
    for (int k = 0; k < n; k += m) {
        std::complex<double> w = 1.0;
        for (int j = 0; j < m / 2; ++j) {
            const auto t      = w * a[k + j + m / 2];
            const auto u      = a[k + j];
            a[k + j]          = u + t;
            a[k + j + m / 2]  = u - t;
            w *= wm;
        }
    }
}
```

**Quant**: Carr-Madan FFT pricing of European options under any model with
a known characteristic function (Heston, Variance-Gamma, etc.).
