// CUDA challenge — batched in-place Cholesky on small SPD matrices, USING
// the shared C primitive `nm_cholesky_inplace` from c_api/cholesky_inplace.h.
//
// SETTING: in a multi-factor stochastic-vol Monte Carlo, each path step
// needs to factor a small (say 5x5) covariance matrix. 1M paths × 100 steps
// = 100M factorizations. Sequential CPU is ~hours; batched GPU is ~seconds.
//
// DESIGN PATTERN — Object Pool: pre-allocate a single device buffer of size
//   n_matrices * n * n; each block accesses its slice; never cudaMalloc per
//   matrix.
//
// TODO 1 — kernel:
//
//   #include "c_api/cholesky_inplace.h"
//
//   __global__ void cholesky_batch(int n, double* As) {
//       extern __shared__ double tile[];
//       double* Ai = As + blockIdx.x * n * n;
//       int     tid = threadIdx.x;
//       // cooperative load
//       for (int k = tid; k < n*n; k += blockDim.x) tile[k] = Ai[k];
//       __syncthreads();
//
//       if (tid == 0) {
//           int rc = nm_cholesky_inplace(tile, n);   // shared with CPU code!
//           if (rc != 0) {
//               /* signal failure: write -1 into the diagonal */
//               for (int j = 0; j < n; ++j) tile[j*n + j] = -1.0;
//           }
//       }
//       __syncthreads();
//
//       for (int k = tid; k < n*n; k += blockDim.x) Ai[k] = tile[k];
//   }
//
//   Note: only thread 0 calls nm_cholesky_inplace because the C function is
//   inherently sequential. For larger n you'd want a tile-cooperative
//   variant; for n <= 16 this is fine.
//
// TODO 2 — host wrapper, validation against `nm::linalg::cholesky` on the
//   first matrix in the batch.
//
// References: cuSOLVER docs (cusolverDnXpotrfBatched is the production path).
