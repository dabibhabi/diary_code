#ifndef NM_C_API_BLAS1_H
#define NM_C_API_BLAS1_H

#include "nm_inline.h"

#include <stddef.h>

/* Level-1 BLAS-style operations on raw pointers.
 *
 * Why C: predictable codegen. The compiler can autovectorize these tight
 * loops without worrying about std::function overhead, exceptions, or
 * iterator abstractions. The __restrict__ qualifier tells it the arrays
 * don't alias.
 *
 * NM_INLINE: also callable from CUDA __device__ code (vector_ops.cu).
 *
 * TODO: implement each.
 */

/* y = a*x + y */
NM_INLINE void nm_saxpy(size_t n, double a, const double* NM_RESTRICT x, double* NM_RESTRICT y) {
    /* TODO: simple for-loop sufficies; compilers will SIMD it. */
    (void)n;
    (void)a;
    (void)x;
    (void)y;
}

/* return x . y */
NM_INLINE double nm_dot(size_t n, const double* NM_RESTRICT x, const double* NM_RESTRICT y) {
    /* TODO. Watch for catastrophic cancellation on huge n; pairwise summation
     * is more accurate than naive accumulation. */
    (void)n;
    (void)x;
    (void)y;
    return 0.0;
}

/* x *= a, in place */
NM_INLINE void nm_scale(size_t n, double a, double* NM_RESTRICT x) {
    /* TODO. */
    (void)n;
    (void)a;
    (void)x;
}

#endif /* NM_C_API_BLAS1_H */
