# `src/` — C++ implementation tree

Each subdirectory has its own README explaining the math and the modern-C++
patterns used. Top-level files (`utils.cpp`, `convergence.cpp`,
`functions.cpp`) are small enough to live without per-file documentation.

## Subdirectory map

| Directory | Purpose | Test labels |
|---|---|---|
| `patterns/` | Strategy / Template Method / Factory / Observer / Builder / Decorator | `iterative_root_finder`, `solver_factory`, `mc_engine_builder`, etc. |
| `c_wrappers/` | C++ wrappers around `c_api/` (RAII over raw pointers) | `cholesky` |
| `dsa/` | Data structures & algorithms (heap, CSR, FFT) | `error_heap`, `csr_sparse`, `fft` |
| `differentiation/` | Numerical derivatives (Richardson, complex-step) | `richardson_derivative`, `complex_step_derivative` |
| `interpolation/` | Cubic splines (natural, monotone, PCHIP) | `natural_cubic_spline`, `monotone_cubic`, `pchip` |
| `linalg/` | Dense LU, QR, CG (Cholesky lives in `c_wrappers/`) | `lu_decompose`, `qr_decompose`, `conjugate_gradient` |
| `ode/` | Forward Euler, RK4, Dormand-Prince RK45 | `euler`, `rk4`, `rk45` |
| `optimization/` | Golden-section, Brent-1D, Nelder-Mead, LM | `golden_section`, `brent_min`, `nelder_mead`, `levenberg_marquardt` |
| `pde/` | Heat-equation finite differences | `heat_explicit`, `heat_implicit`, `crank_nicolson` |
| `quadrature/` | Romberg, adaptive Simpson, Gauss-{Legendre, Hermite}, tanh-sinh | `romberg`, `adaptive_simpson`, `gauss_legendre`, `gauss_hermite`, `tanh_sinh` |
| `random_mc/` | Box-Muller, inv-normal-CDF, Sobol, variance reduction | `box_muller`, `inv_normal_cdf`, `sobol_1d` |

Every algorithm body is currently a stub that throws `nm::not_implemented`
via `NM_TODO("...")`. Tests recognize this and report `[TODO]` rather than
failing CI. Implement, check the TODO box, and CI flips your test on.
