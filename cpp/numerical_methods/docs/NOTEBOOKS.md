# Jupyter notebooks

The `notebooks/` directory holds Python-fronted demonstrations that consume
the `nmpy` pybind11 module. They're not tests — they're how you *experience*
the library after you've implemented enough of it.

## Setup

```bash
# 1. Build with Python bindings
cmake -S . -B build -DNM_USE_PYTHON=ON \
      -Dpybind11_DIR="$(python -m pybind11 --cmakedir)"
cmake --build build

# 2. Install jupyter and matplotlib
python -m pip install jupyterlab matplotlib numpy

# 3. Launch
make notebooks   # or: jupyter lab notebooks/
```

## Notebook index

### `01_root_finding.ipynb` — Convergence rates side-by-side

Runs `BisectionSolver`, `NewtonSolver`, `SecantSolver`, `BrentSolver` on the
same test functions. Attaches a `CapturingObserver` to each, then plots
$|e_k|$ vs $k$ on a log-y axis. Visual confirmation that:

- Bisection: linear convergence $|e_{k+1}| = \tfrac12 |e_k|$
- Newton: quadratic $|e_{k+1}| \sim |e_k|^2$
- Secant: order $\varphi \approx 1.618$
- Brent: superlinear, with bisection-safe worst case

Slug: **nb_root_finding**.

### `02_monte_carlo.ipynb` — Variance reduction in action

Builds three engines via `MonteCarloEngineBuilder`:

1. Plain MC
2. With `AntitheticDecorator`
3. With both `AntitheticDecorator` and `ControlVariateDecorator`

Prices a European call (strike $K$, maturity $T$, vol $\sigma$). Plots
estimator standard error vs number of paths $N$ for each variant.

Math:

$$
\hat C \;=\; \frac{e^{-rT}}{N} \sum_{i=1}^N \max(S_T^{(i)} - K, 0)
$$

Slug: **nb_monte_carlo**.

### `03_pde_pricing.ipynb` — Heat equation → Black-Scholes

Solves $u_t = \nu u_{xx}$ on a grid with `nmpy` finite-difference schemes
(explicit, implicit, Crank-Nicolson). Then applies the change of variables
that turns Black-Scholes into the heat equation:

$$
v(S, t) = e^{-r\tau} \, u(\xi(S, t), \tau)
$$

Plots the resulting price surface and overlays the closed-form formula

$$
C = S_0 \, \Phi(d_1) - K e^{-rT} \, \Phi(d_2).
$$

Slug: **nb_pde_pricing**.

### `04_calibration.ipynb` — Levenberg-Marquardt vol-smile fit

Takes a small chain of market option prices, computes implied vols via the
solvers in §1, then fits a parametric smile (e.g. SVI: $w(k) = a + b\,(\rho(k-m) + \sqrt{(k-m)^2 + s^2})$)
to the implied-vol curve using `nmpy.optimization.levenberg_marquardt`.

This is real quant work in miniature — nonlinear least-squares calibration
is a daily activity at any vol-trading desk.

Slug: **nb_calibration**.

## Why notebooks (and not just tests)

Tests catch correctness regressions; notebooks build *intuition*. You don't
"feel" what quadratic convergence looks like until you see the log-error
plot drop off a cliff.

For CI hygiene, each notebook can be smoke-tested headlessly:

```bash
jupyter nbconvert --to notebook --execute notebooks/01_root_finding.ipynb --output /tmp/out.ipynb
```

If you want notebook-as-test, point CI at this one-liner gated by the
**nb_root_finding** checkbox in TODO.md.
