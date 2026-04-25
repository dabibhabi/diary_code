# `src/patterns/` — design pattern infrastructure

Six GoF patterns plus CRTP. See `docs/PATTERNS.md` for the full class diagram.

## Strategy + Template Method (`iterative_root_finder.cpp`)

Every iterative root-finder is built from one shared loop and a per-method
step rule. The loop fires Observer callbacks and detects convergence:

$$
\text{stop when}\quad |x_{k+1} - x_k| < \epsilon\quad\text{or}\quad k = N_{\max}
$$

Empirical convergence order $p$ from the recorded $|e_k|$:

$$
p \;\approx\; \frac{\log(|e_{k+1}|/|e_k|)}{\log(|e_k|/|e_{k-1}|)}
$$

### Modern C++17 sketch (what you'll write in `solve()`)

```cpp
ConvergenceResult IterativeRootFinder::solve(double tol, int max_iter) {
    ConvergenceResult res{};
    reset_state();
    for (int k = 0; k < max_iter; ++k) {
        double abs_err{}, fx{};
        const double x = do_step(abs_err, fx);          // virtual hook
        res.errors.push_back(abs_err);
        ++res.iterations;
        if (notify({k, x, abs_err, fx}) ||              // Observer fan-out
            is_converged(abs_err, tol)) {
            res.root      = x;
            res.converged = is_converged(abs_err, tol);
            return res;
        }
    }
    res.root = current_x();
    return res;
}
```

Notes on idiom:
- **Default-init via `{}`** instead of `= 0` — works for any type, no narrowing.
- **`const double x = do_step(...)`** — local immutability; compiler can hoist.
- **Structured aggregate return** — `ConvergenceResult{}` zero-inits POD members.

## Factory (`solver_factory.cpp`)

Type-erased construction by enum. Returns `std::unique_ptr<IRootFinder>` so
ownership is explicit and the call site is allocator-free.

```cpp
auto s = SolverFactory::create(SolverKind::NEWTON, cfg);
if (!s) throw std::invalid_argument("incomplete config");
auto r = s->solve(1e-12, 200);
```

## Observer (`convergence_observer.cpp`)

Each observer reacts to an `IterationEvent { k, x_k, |e_k|, f(x_k) }`.

`CapturingObserver` collects events for plotting; `EarlyStopObserver` votes
to halt below a threshold; `LoggingObserver` writes to stderr.

```cpp
auto cap = std::make_shared<CapturingObserver>();
solver.attach(cap);
solver.solve(1e-12, 200);
for (const auto& [k, x, err, fx] : cap->events()) { /* structured bindings */ }
```

## Builder (`mc_engine_builder.cpp`)

Fluent setters + invariant validation in `build()`. The builder rejects
incomplete or contradictory configs:

$$
\text{require:}\quad \text{payoff} \neq \emptyset \;\land\; n_\text{paths} > 0 \;\land\; (\text{cv\_fn},\,\mu_\text{cv})\;\text{both set or neither}
$$

```cpp
auto engine = MonteCarloEngineBuilder()
                  .with_payoff(call_payoff)
                  .with_paths(100'000)             // C++14 digit separators
                  .with_antithetic()
                  .with_control_variate(stock_cv, S0 * std::exp(r*T))
                  .build();                         // throws std::invalid_argument
```

## Decorator (`mc_decorators.cpp`)

Wrap any `IMCEstimator` with variance-reduction. Stackable.

$$
X' = X - \hat\beta\,(Y - \mu_Y),\quad \hat\beta = \frac{\widehat{\mathrm{Cov}}(X,Y)}{\widehat{\mathrm{Var}}(Y)}
$$

```cpp
std::unique_ptr<IMCEstimator> e = std::make_unique<MyMC>(...);
e = std::make_unique<AntitheticDecorator>(std::move(e));
e = std::make_unique<ControlVariateDecorator>(std::move(e), cv_fn, cv_mean);
auto r = e->estimate(100'000, /*seed=*/42);
```

## CRTP (`include/patterns/crtp_root_finder.h` — header-only)

Same shape as `IRootFinder` but compile-time dispatch. The compiler can
inline `do_step()` *and* the user's `f(x)` into `solve()`. Run
`tests/test_crtp_dispatch.cpp` for the microbenchmark.

```cpp
template <class Derived>
class CRTPIterativeRootFinder {
public:
    ConvergenceResult solve(double tol, int max_iter) {
        auto& self = static_cast<Derived&>(*this);
        // ... loop, calls self.do_step() — fully inlined under -O2
    }
};
```
