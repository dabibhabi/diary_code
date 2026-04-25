// CUDA challenge — batched Geometric Brownian Motion paths and a European
// call price via Monte Carlo, USING the shared C primitives.
//
//   #include "c_api/xorshift.h"        // xorshift64_next  (NM_INLINE)
//   #include "c_api/inv_normal_cdf.h"  // nm_inv_normal_cdf (NM_INLINE)
//
// Both expand to __host__ __device__ static inline under nvcc — the same
// algorithm body we use on the CPU runs inside this kernel.
//
// LANGUAGE CHOICE — why CUDA:
//   1M paths × 252 steps = 252M independent trajectories. Each thread owns
//   one path; throughput dominates kernel-launch latency.
//
// DESIGN PATTERN — Strategy via __device__ functor:
//   template <class Payoff>
//   __global__ void price_kernel(... Payoff payoff ...);
//   The payoff is a __device__ functor; specializations for call, put,
//   digital get compiled in. One kernel, three pricers, no virtual calls.
//
// TODO 1 — write the kernel:
//
//   #include "c_api/inv_normal_cdf.h"
//   #include "c_api/xorshift.h"
//
//   template <class Payoff>
//   __global__ void gbm_path_kernel(int n_paths, int n_steps, float S0, float r,
//                                   float sigma, float T, Payoff payoff,
//                                   uint64_t seed, double* out) {
//       int i = blockIdx.x * blockDim.x + threadIdx.x;
//       if (i >= n_paths) return;
//
//       uint64_t state = seed ^ (uint64_t(i) * 0x9E3779B97F4A7C15ULL);
//       float dt = T / n_steps;
//       float drift = (r - 0.5f * sigma * sigma) * dt;
//       float vol   = sigma * sqrtf(dt);
//
//       float S = S0;
//       for (int t = 0; t < n_steps; ++t) {
//           uint64_t r64 = xorshift64_next(&state);
//           double u = xorshift_to_unit(r64);
//           double z = nm_inv_normal_cdf(u);   // shared with CPU code!
//           S *= __expf(drift + vol * (float)z);
//       }
//       out[i] = payoff(S);
//   }
//
// TODO 2 — host wrapper: launch, parallel reduction, discount, divide by n_paths.
// TODO 3 — verify against analytic Black-Scholes for a fixed seed.
//
// QUANT NOTES:
//   - This is the bones of every multi-asset Monte Carlo pricer. Add a
//     Cholesky-decomposed correlation, replace one S with a vector of
//     per-asset spots, and you have correlated GBM.
//   - Use cuRAND's Sobol generator if you want low-discrepancy paths
//     (better convergence — Brownian bridge / PCA construction is the
//     right way for high-dim QMC).
//
// References: Glasserman ch. 3; cuRAND library docs.
