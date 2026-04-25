# `src/quadrature/` — numerical integration

Five methods, two families:

- **Newton-Cotes / extrapolation**: trapezoid → Simpson → Romberg
  (uniform sample points; high-order via Richardson)
- **Gaussian**: choose nodes optimally for a given weight function

## Romberg (`romberg.cpp`)

Repeated trapezoid + Richardson:

$$
T_{k,0} \;=\; \text{trapezoid with}\;2^k\,\text{subintervals},\qquad
T_{k,j} \;=\; \frac{4^j\,T_{k,j-1} - T_{k-1,j-1}}{4^j - 1}
$$

The diagonal $T_{k,k}$ converges as $O(h^{2(k+1)})$ for smooth integrands.
Stop when $|T_{k,k} - T_{k-1,k-1}| < \epsilon$.

### Modern C++17 sketch — refine trapezoid only on new midpoints

```cpp
double romberg(func f, double a, double b, int max_levels, double tol) {
    std::vector<std::vector<double>> T;
    T.reserve(max_levels);
    T.push_back({0.5 * (b - a) * (f(a) + f(b))});         // T[0][0]

    for (int k = 1; k < max_levels; ++k) {
        const int    n  = 1 << (k - 1);                     // new midpoints
        const double h  = (b - a) / (1 << k);
        double       s  = 0.0;
        for (int i = 0; i < n; ++i) s += f(a + (2 * i + 1) * h);
        std::vector<double> row(k + 1);
        row[0] = 0.5 * T[k - 1][0] + h * s;                 // T[k][0]
        for (int j = 1; j <= k; ++j) {
            const double pow4 = static_cast<double>(1 << (2 * j));
            row[j] = (pow4 * row[j - 1] - T[k - 1][j - 1]) / (pow4 - 1.0);
        }
        if (std::abs(row.back() - T.back().back()) < tol) return row.back();
        T.push_back(std::move(row));
    }
    return T.back().back();
}
```

## Adaptive Simpson (`adaptive_simpson.cpp`)

Recursively split where Simpson's rule disagrees with itself:

$$
S(a,b) \;=\; \tfrac{b-a}{6}\big(f(a) + 4 f(\tfrac{a+b}{2}) + f(b)\big)
$$

Accept on $[a,b]$ when $|S(a,m) + S(m,b) - S(a,b)| < 15\,\epsilon$ (the 15
comes from Richardson on the embedded estimate). Otherwise recurse on each
half with $\epsilon/2$.

The **heap-based** variant (`dsa::ErrorHeap`) is strictly better for
non-uniform integrands — it spends evaluations where they're needed first.

## Gauss-Legendre (`gauss_legendre.cpp`)

The $n$-point rule integrates polynomials of degree $\le 2n - 1$ exactly
on $[-1, 1]$. Map to $[a, b]$ via $t_i = \frac{b-a}{2}\,x_i + \frac{a+b}{2}$:

$$
\int_a^b f(x)\,dx \;\approx\; \frac{b-a}{2}\sum_{i=1}^n w_i\,f(t_i)
$$

For $n \in \{2, 4, 8, 16\}$ tabulate the nodes/weights; for general $n$,
compute via Newton's method on $P_n(x) = 0$ (NR3 §4.6).

## Gauss-Hermite (`gauss_hermite.cpp`)

For integrals against the Gaussian weight $e^{-x^2}$:

$$
\int_{-\infty}^{\infty} e^{-x^2}\,g(x)\,dx \;\approx\; \sum_{i=1}^n w_i\,g(x_i)
$$

To compute $\mathbb{E}[g(Z)]$ under $Z \sim \mathcal{N}(0, 1)$, use the
substitution $x = z/\sqrt{2}$:

$$
\mathbb{E}[g(Z)] \;=\; \frac{1}{\sqrt{\pi}} \sum_{i=1}^n w_i\,g(\sqrt{2}\,x_i)
$$

**Quant**: this is THE quadrature for finance. Bachelier pricing, normal-SABR
caplets, Black-Scholes with stochastic vol approximations — all reduce to
$\mathbb{E}[g(Z)]$ for some closed-form $g$, and $n = 16$ already gives
14-digit accuracy.

## Tanh-sinh (`tanh_sinh.cpp`)

For integrals with endpoint singularities. Substitute

$$
x = \tanh\!\big(\tfrac{\pi}{2}\,\sinh t\big)
$$

so the integrand decays double-exponentially at $t = \pm\infty$. A uniform
trapezoid rule in $t$ then converges very fast even when $f$ blows up at
$x = \pm 1$. Bailey & Borwein (free PDF, "Tanh-Sinh High-Precision
Quadrature") is the canonical reference.
