# CUDA challenges

Each `.cu` here is a stub describing a method that **maps cleanly to a GPU**.
The whole point: pick the right tool for the right shape of work.

## Decision tree (when to reach for CUDA)

| Shape of work | Best language | Why |
|---|---|---|
| One scalar root-find / 1D quadrature / small linsolve | **C++** | Latency dominates; kernel launch alone is 10 µs |
| Cache-tight inner loops on a single CPU thread | **C** | Smallest code, predictable codegen — useful when called from Python via raw ctypes |
| Building a clean library API with templates / RAII / `std::function` | **C++** | What this repo's host code is |
| Same kernel applied to **millions** of independent items | **CUDA** | Each thread = one item; throughput dominates latency |
| Sparse / irregular data structures | **C++** + maybe MPI | GPU thread divergence kills perf |
| Tridiagonal / sequential per-step solves on a single grid | **C++** | Inherently sequential |
| Tridiagonal **batched** (one per asset, thousands of assets) | **CUDA** (cuSPARSE `gtsv2`) | Across-batch parallelism |

## The challenges

### 1. `monte_carlo_paths.cu` — batched GBM Monte Carlo
- Each CUDA thread simulates one path of `dS = μ S dt + σ S dW`.
- Use cuRAND's `curand_normal` per step.
- Reduction at the end: parallel sum of payoffs → option price + standard error.
- **Design pattern angle:** apply the *Strategy* pattern at the host level — the
  payoff is a `__device__` functor passed as a template parameter. Same kernel,
  different payoffs (call/put/digital) at compile time.
- **Why CUDA, not C++ threads:** 1M paths × 252 days = 252M independent
  trajectories; a CPU at 8 cores gets ~10 GFlops, an A100 gets ~10 TFlops.

### 2. `pde_heat_explicit.cu` — explicit FTCS on the GPU
- Each thread updates one grid point per time step.
- Use shared memory: load a tile (with halo) into `__shared__`, sync, update.
- Pingpong two device buffers across time steps.
- **Why CUDA:** the update at `i, n+1` only reads `i-1, i, i+1` at step `n`. Fully
  data-parallel across `i`. CPU SIMD gets ~4×; GPU gets ~100×.

### 3. `vector_ops.cu` — saxpy + dot product as a warm-up
- `saxpy(a, x, y)`: `y = a*x + y`. The "hello world" of CUDA.
- `dot(x, y)`: parallel reduction. Two-pass: per-block partial sums in
  shared memory, then a final reduction kernel.
- **Why include this:** every harder kernel uses these patterns
  (coalesced loads, shared memory, warp reductions).

### 4. `cholesky_batch.cu` — many small SPD matrices factorized at once
- One CUDA block per matrix; threads collaborate on the columns.
- Use shared memory for the matrix tile.
- **Why CUDA:** in a multi-asset stochastic-vol model you re-factor the
  per-time-step covariance for hundreds of paths; batch-Cholesky on the GPU
  is the common move.

## How to build & run

```bash
cmake -S . -B build -DNM_USE_CUDA=ON     # opt in to CUDA
cmake --build build
ctest --test-dir build -L cuda            # only the CUDA tests
```

If `nvcc` isn't on the path, the `NM_USE_CUDA` option silently ignores the
CUDA targets — the rest of the project still builds.
