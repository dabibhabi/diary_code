#ifndef NM_C_API_CHOLESKY_INPLACE_H
#define NM_C_API_CHOLESKY_INPLACE_H

#include "nm_inline.h"

#include <math.h>

#define NM_CHOLESKY_MAX_N 16

/* In-place Cholesky factorization for small SPD matrices (n <= 16).
 *
 * Why fixed-size limit: above ~16, register pressure on the GPU starts
 * hurting; you want a tile-based approach with shared memory and would
 * prefer cuSOLVER's cusolverDnXpotrfBatched. For n <= 16, the same code
 * runs cleanly on CPU and inside a CUDA __device__ block (one matrix
 * per thread block).
 *
 * Layout: row-major; A[i*n + j].
 * Output: lower triangle of A overwritten with L (s.t. A = L L^T);
 *         upper triangle is unchanged (caller can zero it if desired).
 * Returns 0 on success, -1 if a non-positive pivot is encountered.
 *
 * TODO: implement.
 */
NM_INLINE int nm_cholesky_inplace(double* A, int n) {
    if (n <= 0 || n > NM_CHOLESKY_MAX_N) return -1;
    /* TODO:
     *   for (int j = 0; j < n; ++j) {
     *       double s = A[j*n + j];
     *       for (int k = 0; k < j; ++k) s -= A[j*n + k] * A[j*n + k];
     *       if (s <= 0.0) return -1;
     *       A[j*n + j] = sqrt(s);
     *       for (int i = j+1; i < n; ++i) {
     *           double t = A[i*n + j];
     *           for (int k = 0; k < j; ++k) t -= A[i*n + k] * A[j*n + k];
     *           A[i*n + j] = t / A[j*n + j];
     *       }
     *   }
     */
    (void)A;
    return -1;
}

#endif /* NM_C_API_CHOLESKY_INPLACE_H */
