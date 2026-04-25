# `src/optimization/` — function minimization

Four methods, ranging from 1-D to nonlinear least-squares calibration.

## Golden-section search (`golden_section.cpp`)

1-D, derivative-free, bracket-based. Each step shrinks the bracket by the
inverse golden ratio:

$$
\varphi^{-1} = \frac{\sqrt{5} - 1}{2} \approx 0.618
$$

Place test points at $a + (1-\varphi^{-1})(b-a)$ and $b - (1-\varphi^{-1})(b-a)$;
keep the half containing the smaller value.

```cpp
double golden_section(func f, double a, double b, double tol) {
    constexpr double phi    = 1.6180339887498949;
    constexpr double resphi = 2.0 - phi;          // 1 - 1/phi
    double x1 = a + resphi * (b - a), x2 = b - resphi * (b - a);
    double f1 = f(x1), f2 = f(x2);
    while (std::abs(b - a) > tol) {
        if (f1 < f2) {
            b = x2; x2 = x1; f2 = f1;
            x1 = a + resphi * (b - a); f1 = f(x1);  // reuses one eval per iter
        } else {
            a = x1; x1 = x2; f1 = f2;
            x2 = b - resphi * (b - a); f2 = f(x2);
        }
    }
    return 0.5 * (a + b);
}
```

## Brent's 1-D minimization (`brent_min.cpp`)

Combines golden-section (safe, slow) with parabolic interpolation through
the three best points (fast when usable). Same spirit as Brent's root-finder.
NR3 §10.3 has the canonical pseudocode (Algorithm BRENT).

## Nelder-Mead simplex (`nelder_mead.cpp`)

n-D, derivative-free. Maintains an $n+1$-vertex simplex and applies
**reflect / expand / contract / shrink** based on how the worst vertex
compares to the rest. Heuristic but robust.

The parameter set is well-known:

$$
\rho = 1\;\;(\text{reflect}),\quad \chi = 2\;\;(\text{expand}),\quad
\gamma = 0.5\;\;(\text{contract}),\quad \sigma = 0.5\;\;(\text{shrink})
$$

## Levenberg-Marquardt (`levenberg_marquardt.cpp`)

Nonlinear least-squares. Minimize

$$
\Phi(x) \;=\; \tfrac12 \sum_i r_i(x)^2 \;=\; \tfrac12 \|r(x)\|_2^2
$$

by solving (at each iterate)

$$
(J^{\!\top} J + \lambda\,\mathrm{diag}(J^{\!\top} J))\,\Delta x \;=\; -J^{\!\top} r
$$

If $\lambda \to 0$: Gauss-Newton (fast, near optimum). If $\lambda \to \infty$:
gradient descent (safe, far from optimum). LM adapts $\lambda$ each step:
divide by 10 on success, multiply by 10 on rejection.

### Modern C++17 sketch (one outer iteration)

```cpp
const Vector r  = residuals(x);
const Matrix J  = jacobian(x);
const Matrix JtJ = matmul_transpose(J, J);    // J^T J
const Vector Jtr = matvec_transpose(J, r);    // J^T r

Matrix M = JtJ;
for (int i = 0; i < M.rows; ++i) M(i, i) += lambda * JtJ(i, i);

const Vector dx = solve_via_cholesky(M, neg(Jtr));
const Vector x_new = add(x, dx);

if (cost(x_new) < cost(x)) { x = x_new; lambda *= 0.1; }
else                         { lambda *= 10.0; }
```

Notes:
- **The hot solve is SPD** ($J^{\!\top}J + \lambda\,\mathrm{diag}$), so reuse
  `linalg::cholesky` from this library — that's the design payoff: building
  a higher method out of building blocks you already wrote.
- **Damping update** is a *trust-region* style strategy; many production
  variants (Marquardt's original, Moré's, Fletcher's) tweak the rule.

**Quant**: this is the calibration workhorse. SABR / Heston / SVI fits to
implied-vol surfaces are all least-squares fits, and LM is what desks
actually use. Notebook `04_calibration.ipynb` walks through one.
