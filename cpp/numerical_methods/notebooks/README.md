# Notebooks

Python frontends that consume the `nmpy` pybind11 module. Build with
`cmake -DNM_USE_PYTHON=ON` first; the `conftest.py` shim under
`tests/python/` shows how `nmpy` gets onto `sys.path`.

| Notebook | Slug | Demonstrates |
|---|---|---|
| `01_root_finding.ipynb` | `nb_root_finding` | Convergence-rate plots from `CapturingObserver` |
| `02_monte_carlo.ipynb` | `nb_monte_carlo` | MC variance reduction (Builder + Decorator) |
| `03_pde_pricing.ipynb` | `nb_pde_pricing` | Heat equation $\to$ Black-Scholes |
| `04_calibration.ipynb` | `nb_calibration` | Levenberg-Marquardt vol-smile fit |

See `docs/NOTEBOOKS.md` for what each notebook teaches in detail.

The notebooks are intentionally minimal scaffolds — markdown cells outline
the experiment, code cells have TODO placeholders. They're a good test of
the OO API once you've implemented `IterativeRootFinder::solve()`,
`MonteCarloEngineBuilder::build()`, and the underlying algorithms.
