#ifndef NM_C_API_XORSHIFT_H
#define NM_C_API_XORSHIFT_H

#include "nm_inline.h"

#include <stdint.h>

/* xorshift64 — one of the simplest fast PRNGs. Period 2^64 - 1.
 * Marsaglia (2003), "Xorshift RNGs". Adequate for Monte Carlo where you need
 * speed and statistical quality is checked at a higher level.
 *
 * Why C+NM_INLINE: same code runs on CPU (in c/xorshift.c) and inside CUDA
 * __device__ kernels (in cuda/monte_carlo_paths.cu). One source of truth.
 *
 * TODO: implement.
 *   state ^= state << 13;
 *   state ^= state >> 7;
 *   state ^= state << 17;
 *   return state;
 */
NM_INLINE uint64_t xorshift64_next(uint64_t* state) {
    /* TODO: implement the three xorshift steps and return the new state. */
    (void)state;
    return 0;
}

/* Convert a uint64 to a double in [0,1). Used to feed inv_normal_cdf. */
NM_INLINE double xorshift_to_unit(uint64_t r) {
    /* TODO: implement using the top 53 bits of r divided by 2^53. */
    (void)r;
    return 0.0;
}

#endif /* NM_C_API_XORSHIFT_H */
