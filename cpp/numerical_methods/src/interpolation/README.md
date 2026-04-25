# `src/interpolation/` — 1-D cubic interpolation

Three flavors of cubic spline. The data structure is the same — a knot
sequence $\{x_i\}$ and per-segment coefficients $\{a_i, b_i, c_i, d_i\}$
— but the *slopes* are chosen differently.

## On segment $[x_i, x_{i+1}]$, with $t = x - x_i$:

$$
S_i(t) \;=\; a_i + b_i\,t + c_i\,t^2 + d_i\,t^3
$$

## Natural cubic spline (`cubic_spline.cpp`)

Solve a tridiagonal system for the second derivatives $M_i$ with the
*natural* boundary $M_0 = M_{n-1} = 0$:

$$
\frac{h_{i-1}}{6}M_{i-1} + \frac{h_{i-1}+h_i}{3}M_i + \frac{h_i}{6}M_{i+1}
\;=\; \frac{y_{i+1}-y_i}{h_i} - \frac{y_i - y_{i-1}}{h_{i-1}}
$$

where $h_i = x_{i+1} - x_i$. Then convert $(M, x, y)$ into per-segment
$(a, b, c, d)$.

### Modern C++17 evaluation sketch

```cpp
double CubicSpline::operator()(double xq) const {
    // Binary search for the segment in O(log n).
    auto it = std::upper_bound(x.begin(), x.end(), xq);
    const std::size_t i =
        (it == x.begin())   ? 0
      : (it == x.end())     ? x.size() - 2
                            : std::distance(x.begin(), it) - 1;
    const double t = xq - x[i];
    return a[i] + t * (b[i] + t * (c[i] + t * d[i]));   // Horner form
}
```

Notes:
- **`std::upper_bound` + `std::distance`** is the canonical O(log n) lookup.
- **Horner's form** for the polynomial evaluation — one mul-add per power.
- **`auto it`** + **`std::size_t`** keeps the index types correct.

## Monotone cubic — Hyman / Steffen (`monotone_cubic.cpp`)

Plain cubic splines can produce spurious oscillations: between two close
data points, the spline can shoot above/below the values. For yield-curve
construction this is **bad** — it can produce negative forward rates.

Hyman / Steffen filter forces:

$$
\mathrm{sign}(\delta_{i-1}) \neq \mathrm{sign}(\delta_i) \;\Longrightarrow\; m_i = 0
$$

and clamps:

$$
|m_i| \;\le\; 3\,\min(|\delta_{i-1}|, |\delta_i|),\quad
\delta_i \;=\; \frac{y_{i+1}-y_i}{x_{i+1}-x_i}
$$

**Quant**: Hagan & West, "Interpolation Methods for Curve Construction"
(Applied Math Finance 2006), is the canonical reference.

## PCHIP — Fritsch-Carlson (`pchip.cpp`)

Same monotone-preservation goal, slightly different slope formula —
weighted harmonic mean of neighbour secants:

$$
m_i \;=\; \frac{w_1 + w_2}{w_1/\delta_{i-1} + w_2/\delta_i},\quad
w_1 = 2h_i + h_{i-1},\quad w_2 = h_i + 2h_{i-1}
$$

If $\delta_{i-1}$ and $\delta_i$ have opposite signs, set $m_i = 0$.

This is what MATLAB's `pchip` and SciPy's `PchipInterpolator` use.

```cpp
auto pchip_slopes = [&](std::span<const double> x, std::span<const double> y) {
    // C++20 std::span gives a non-owning view; works the same with
    // const std::vector<double>&.
    std::vector<double> m(x.size(), 0.0);
    // ... TODO: compute Fritsch-Carlson slopes
    return m;
};
```
