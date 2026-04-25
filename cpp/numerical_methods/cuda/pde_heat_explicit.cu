// CUDA challenge — explicit FTCS scheme for the 1-D heat equation.
//
//   u_t = ν u_xx     →     u_i^{n+1} = u_i^n + r (u_{i-1}^n - 2 u_i^n + u_{i+1}^n)
//   r = ν dt / dx^2,  STABILITY: r ≤ 1/2.
//
// LANGUAGE CHOICE:
//   - Small grid (N ≤ 1024) → CPU. The PDE is tiny; CPU L1 cache holds the
//     whole grid and SIMD finishes in micro-seconds. CUDA launch overhead
//     dominates.
//   - Large grid (N ≥ 1e5) or 2-D / 3-D PDE → CUDA wins decisively. Each
//     thread updates one grid point; the stencil is fully data-parallel
//     across spatial index at a fixed time level.
//
// DESIGN PATTERN — *Double Buffering* (a.k.a. ping-pong):
//   - Allocate two device arrays, swap pointers each step.
//   - Avoids any in-place write hazards (a thread mustn't read a neighbor
//     that another thread has already overwritten in the same step).
//
// TODO 1 — kernel using shared memory for the halo:
//   __global__ void ftcs_step(int N, float r, const float* u, float* u_new) {
//       __shared__ float tile[BLOCK_SIZE + 2];     // +2 for halo cells
//       int i = blockIdx.x * blockDim.x + threadIdx.x;
//       int li = threadIdx.x + 1;                  // local index inside tile
//
//       if (i < N) tile[li] = u[i];
//       if (threadIdx.x == 0)
//           tile[0] = (i > 0) ? u[i - 1] : 0.0f;   // left halo or BC
//       if (threadIdx.x == blockDim.x - 1 || i == N - 1)
//           tile[li + 1] = (i < N - 1) ? u[i + 1] : 0.0f;  // right halo or BC
//       __syncthreads();
//
//       if (i > 0 && i < N - 1) {
//           u_new[i] = tile[li] + r * (tile[li-1] - 2.0f * tile[li] + tile[li+1]);
//       } else if (i == 0 || i == N - 1) {
//           u_new[i] = u[i];                       // Dirichlet BC pinned
//       }
//   }
//
// TODO 2 — host wrapper: allocate u and u_new, copy initial condition,
//   loop M time steps with cudaMemcpy/swap, copy final u back.
//
// TODO 3 — verify against pde::heat_explicit (the CPU version you'll
//   implement). For an initial sin(πx) on [0,1], the analytic solution is
//   sin(πx) * exp(-π² ν t) — error should match O(dt) + O(dx²).
//
// References:
//   - NR3 §20.2 ("Diffusive Initial Value Problems").
//   - Sanders & Kandrot, "CUDA by Example", ch. 7 (heat transfer).
