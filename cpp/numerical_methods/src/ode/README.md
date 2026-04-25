# `src/ode/` — ordinary differential equations

Solve $\dot y = f(t, y)$ with $y(t_0) = y_0$ on $[t_0, t_\text{end}]$.

State is `std::vector<double>`; the RHS is a `std::function`:

```cpp
using State = std::vector<double>;
using RHS   = std::function<State(double, const State&)>;
```

## Forward Euler (`euler.cpp`)

$$
y_{n+1} \;=\; y_n + h\,f(t_n, y_n),\qquad \text{local error } O(h^2)
$$

Cheapest and least accurate. Conditionally stable (fails on stiff problems).

```cpp
Trajectory euler(RHS f, double t0, State y0, double t_end, double h) {
    Trajectory tr;
    tr.t.push_back(t0); tr.y.push_back(y0);
    State y = std::move(y0);
    for (double t = t0; t < t_end; t += h) {
        const State k = f(t, y);
        for (std::size_t i = 0; i < y.size(); ++i) y[i] += h * k[i];
        tr.t.push_back(std::min(t + h, t_end));
        tr.y.push_back(y);
    }
    return tr;
}
```

## Classical Runge-Kutta 4 (`rk4.cpp`)

Four stages:

$$
\begin{aligned}
k_1 &= f(t_n, y_n), \\
k_2 &= f(t_n + h/2,\; y_n + (h/2)\,k_1), \\
k_3 &= f(t_n + h/2,\; y_n + (h/2)\,k_2), \\
k_4 &= f(t_n + h,\;\; y_n + h\,k_3), \\
y_{n+1} &= y_n + \tfrac{h}{6}\,(k_1 + 2 k_2 + 2 k_3 + k_4).
\end{aligned}
$$

Local error $O(h^5)$, global $O(h^4)$. The non-stiff workhorse.

### Modern C++17 sketch — vector-aware step helper

```cpp
namespace {
inline State axpy(double a, const State& x, const State& y) {
    State out(x.size());
    for (std::size_t i = 0; i < x.size(); ++i) out[i] = a * x[i] + y[i];
    return out;
}
}  // namespace

Trajectory rk4(RHS f, double t0, State y0, double t_end, double h) {
    Trajectory tr;
    tr.t.push_back(t0); tr.y.push_back(y0);
    State y = std::move(y0);
    for (double t = t0; t < t_end; t += h) {
        const State k1 = f(t,           y);
        const State k2 = f(t + h / 2.0, axpy(h / 2.0, k1, y));
        const State k3 = f(t + h / 2.0, axpy(h / 2.0, k2, y));
        const State k4 = f(t + h,       axpy(h,       k3, y));
        for (std::size_t i = 0; i < y.size(); ++i)
            y[i] += h / 6.0 * (k1[i] + 2.0 * k2[i] + 2.0 * k3[i] + k4[i]);
        tr.t.push_back(std::min(t + h, t_end));
        tr.y.push_back(y);
    }
    return tr;
}
```

## Dormand-Prince adaptive RK45 (`rk45.cpp`)

Embedded 4(5) pair: 6 stages give two estimates (4th and 5th order); their
difference is the local error estimate, used to grow/shrink $h$:

$$
\hat e \;=\; \|y_5 - y_4\|,\qquad
h_\text{new} \;=\; 0.9 \cdot h \cdot \left(\frac{\epsilon}{\hat e}\right)^{\!1/5}
$$

If $\hat e \le \epsilon$, accept and grow $h$; else reject and shrink. This
is what MATLAB's `ode45` and SciPy's `RK45` do.

The Butcher tableau (DOPRI5) is fixed and well documented (NR3 §17.2,
Wikipedia "Dormand-Prince_method"). The challenge is the step-size
controller, not the stages.
