# `src/pde/` — finite-difference solvers for the heat equation

Solve

$$
\frac{\partial u}{\partial t} \;=\; \nu\,\frac{\partial^2 u}{\partial x^2}
\quad\text{on}\quad x \in [x_\text{min}, x_\text{max}],\;t \in [0, T],
$$

with Dirichlet boundary functions $u(x_\text{min}, t)$ and $u(x_\text{max}, t)$
and an initial condition $u(x, 0) = u_0(x)$.

**Quant relevance**: Black-Scholes reduces to the heat equation by a change
of variables. Once these schemes work, you have a real PDE option pricer.

## Discretization

$\Delta x = (x_\text{max} - x_\text{min})/N$ ; $\Delta t = T/M$ ; let
$r = \nu\,\Delta t / \Delta x^2$.

| Scheme | Stencil | Stability | Accuracy |
|---|---|---|---|
| Explicit (FTCS) | $u_i^{n+1} = u_i^n + r\,(u_{i-1}^n - 2u_i^n + u_{i+1}^n)$ | $r \le \tfrac12$ | $O(\Delta t + \Delta x^2)$ |
| Implicit (BTCS) | $-r\,u_{i-1}^{n+1} + (1+2r)\,u_i^{n+1} - r\,u_{i+1}^{n+1} = u_i^n$ | unconditional | $O(\Delta t + \Delta x^2)$ |
| Crank-Nicolson | average of explicit + implicit | unconditional | $O(\Delta t^2 + \Delta x^2)$ |

## Explicit FTCS (`heat_explicit.cpp`)

Each grid point at step $n+1$ depends only on its three neighbours at step
$n$. Embarrassingly parallel — see `cuda/pde_heat_explicit.cu` for the GPU
version.

```cpp
HeatGrid heat_explicit(double nu, double x_min, double x_max, double T,
                       int N, int M, InitialFn u0,
                       BoundaryFn left, BoundaryFn right) {
    const double dx = (x_max - x_min) / N, dt = T / M;
    const double r  = nu * dt / (dx * dx);
    if (r > 0.5)
        throw std::domain_error("heat_explicit: r > 1/2 (unstable)");

    HeatGrid g;
    g.x.resize(N + 1);
    for (int i = 0; i <= N; ++i) g.x[i] = x_min + i * dx;
    g.u.resize(N + 1);
    for (int i = 0; i <= N; ++i) g.u[i] = u0(g.x[i]);

    std::vector<double> u_new(N + 1);
    for (int n = 0; n < M; ++n) {
        const double t      = (n + 1) * dt;
        u_new.front()       = left(t);
        u_new.back()        = right(t);
        for (int i = 1; i < N; ++i)
            u_new[i] = g.u[i] + r * (g.u[i - 1] - 2.0 * g.u[i] + g.u[i + 1]);
        std::swap(g.u, u_new);
    }
    return g;
}
```

Notes:
- **`std::swap` of vectors** = pointer swap, $O(1)$. Standard double-buffer trick.
- **Throw on $r > 1/2$** — better than silently returning garbage.

## Implicit (`heat_implicit.cpp`) and Crank-Nicolson (`crank_nicolson.cpp`)

Both reduce to a tridiagonal solve per time step. Use the strongly-typed
`nm::dsa::Tridiag` from `dsa/tridiag.h` (which calls `c_api/tridiag.h`'s
Thomas algorithm). Per step:

```cpp
Tridiag T;
T.sub  .assign(N + 1, -r);   // adjust at boundaries
T.diag .assign(N + 1, 1.0 + 2.0 * r);
T.super.assign(N + 1, -r);
T.sub.front() = T.super.back() = 0.0;
// boundary contributions go into rhs:
auto rhs = g.u;
rhs.front() = left(t_next);
rhs.back()  = right(t_next);
g.u = nm::dsa::solve(T, rhs);
```

For Crank-Nicolson, the LHS coefficients are $(-r/2, 1+r, -r/2)$ and the
RHS is the explicit half-step:

$$
\text{rhs}_i \;=\; \tfrac{r}{2}\,u_{i-1}^n + (1-r)\,u_i^n + \tfrac{r}{2}\,u_{i+1}^n
$$

**Caveat — Rannacher start**: Crank-Nicolson can oscillate near non-smooth
payoffs (digital options, knock-outs). Standard fix: do 2-4 fully implicit
steps with $\Delta t / 2$ first, then switch to CN. Wilmott vol. 3 has the
details.
