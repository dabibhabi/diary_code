// CUDA challenge — vector ops using the shared `c_api/blas1.h` helpers
// inside __device__ code.
//
// LANGUAGE CHOICE: small N → CPU; 10M+ elements → GPU (memory-bandwidth-bound).
//
// DESIGN PATTERN — Facade: host-side wrappers (saxpy_host, dot_host) hide
// device pointers and stream management from the caller.
//
// TODO 1 — saxpy kernel: y = a*x + y.
//
//   #include "c_api/blas1.h"
//
//   __global__ void saxpy_kernel(size_t n, double a, const double* x, double* y) {
//       size_t i = blockIdx.x * blockDim.x + threadIdx.x;
//       if (i < n) y[i] = a * x[i] + y[i];
//       // Or, leveraging the shared C primitive on a per-thread tile:
//       //   nm_saxpy(1, a, &x[i], &y[i]);
//   }
//
// TODO 2 — dot product with parallel reduction:
//   per-block partial sums in shared memory; second kernel reduces to scalar.
//
// References: Mark Harris, "Optimizing Parallel Reduction in CUDA".
