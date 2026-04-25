#ifndef NM_C_API_NM_INLINE_H
#define NM_C_API_NM_INLINE_H

/* The shared-core macro.
 *   In C/C++ TUs:           NM_INLINE expands to `static inline`
 *   In CUDA (.cu) TUs:      NM_INLINE expands to `__host__ __device__ static inline`
 *
 * Pattern: an algorithm body lives in a header and uses NM_INLINE so that
 *   - c/<name>.c       — picks up the CPU-side definition (when .c TU includes the header)
 *   - cuda/<file>.cu   — picks up the device-callable definition
 *   - C++ wrappers     — include the header and call the function from host code
 *
 * Limits documented in docs/LANGUAGE_CHOICES.md: works well for branch-light,
 * fixed-size or small-N algorithms. Don't try to share large algorithms with
 * heavy register pressure or recursive calls.
 */

#if defined(__CUDACC__)
#define NM_INLINE __host__ __device__ static inline
#else
#define NM_INLINE static inline
#endif

#ifdef __cplusplus
#define NM_RESTRICT __restrict__
#else
#define NM_RESTRICT restrict
#endif

#endif /* NM_C_API_NM_INLINE_H */
