# `src/random_mc/` — random sampling and Monte Carlo

This C++ side hosts the high-level wrappers; the actual leaf math
(`xorshift`, `inv_normal_cdf`) lives in `c_api/` so it can also run inside
CUDA `__device__` code.

## Box-Muller (`box_muller.cpp`)

Two independent $U(0,1)$ samples $\to$ two independent $\mathcal{N}(0,1)$:

$$
Z_1 \;=\; \sqrt{-2\ln U_1}\,\cos(2\pi U_2),\qquad
Z_2 \;=\; \sqrt{-2\ln U_1}\,\sin(2\pi U_2)
$$

```cpp
NormalPair box_muller(double u1, double u2) {
    const double r     = std::sqrt(-2.0 * std::log(u1));
    const double theta = 2.0 * M_PI * u2;
    const auto [c, s]  = std::pair{std::cos(theta), std::sin(theta)};
    return {r * c, r * s};                          // aggregate brace return
}
```

Notes:
- **`std::pair` + structured binding** for the cos/sin pair — purely cosmetic
  here, useful in larger functions.
- **Ziggurat / Marsaglia polar** are faster alternatives; Box-Muller is the
  pedagogical baseline.

## Inverse normal CDF (`inv_normal_cdf.cpp` — but lives in `c_api/`)

Acklam's rational approximation of $\Phi^{-1}(u)$:

$$
\Phi^{-1}(u) \;\approx\; \begin{cases}
\dfrac{a_0 + a_1 q + a_2 q^2 + a_3 q^3 + a_4 q^4 + a_5 q^5}
      {1 + b_0 q + b_1 q^2 + b_2 q^3 + b_3 q^4 + b_4 q^5} & u \in (0.02425,\,0.97575) \\[6pt]
\text{lower-tail rational in } q = \sqrt{-2 \ln u} & u < 0.02425 \\[2pt]
\text{upper-tail rational in } q = \sqrt{-2 \ln(1-u)} & u > 0.97575
\end{cases}
$$

(The constants are listed on the Acklam algorithm page.) Accuracy is
$\approx 10^{-15}$ across $(0, 1)$.

**Why this matters for quant MC**: Box-Muller destroys low-discrepancy
structure, so Sobol-driven Monte Carlo needs `inv_normal_cdf` instead.

## Sobol — 1-D Van der Corput (`sobol.cpp`)

The 1-D Sobol sequence is the bit-reversal of $\{1, 2, \ldots\}$ divided by
$2^k$:

$$
\phi_2(i) \;=\; \frac{\mathrm{bit\_reverse}(i, k)}{2^k}
$$

The first 8 points are $\{0.5, 0.25, 0.75, 0.125, 0.625, 0.375, 0.875, 0.0625\}$.

For higher dimensions you need *direction numbers* — Joe & Kuo tables
provide them up to dimension 21201, free online.

## Variance reduction (`variance_reduction.cpp`)

Glasserman ch. 4. Two flavours:

**Antithetic**: pair $(Z, -Z)$. For monotone payoffs the variance halves
(roughly):

$$
\mathrm{Var}\!\left(\tfrac{X(Z) + X(-Z)}{2}\right) \;\le\;
\tfrac12\,\mathrm{Var}(X(Z))
$$

**Control variate**: subtract a correlated estimator with known mean:

$$
X' \;=\; X - \hat\beta\,(Y - \mu_Y),\quad
\hat\beta \;=\; \frac{\widehat{\mathrm{Cov}}(X, Y)}{\widehat{\mathrm{Var}}(Y)}
$$

The variance of $X'$ is $\mathrm{Var}(X)\,(1 - \hat\rho_{XY}^2)$, so the
better $Y$ correlates with $X$, the bigger the gain. Classical choice for
Black-Scholes MC: $Y = S_T$ (terminal stock price), $\mu_Y = S_0\,e^{rT}$.

These both fit the **Decorator pattern** (`include/patterns/mc_decorators.h`)
— stack them onto any `IMCEstimator`. See `notebooks/02_monte_carlo.ipynb`
for the variance-vs-paths plot.
