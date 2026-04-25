#ifndef NM_C_API_INV_NORMAL_CDF_H
#define NM_C_API_INV_NORMAL_CDF_H

#include "nm_inline.h"

#include <math.h>

/* Acklam's inverse standard normal CDF: Phi^{-1}(u) for u in (0,1).
 * Accuracy: ~1e-15 across the full range.
 *
 * Why C+NM_INLINE: this is the workhorse for Sobol-driven Monte Carlo on
 * the GPU. Sharing keeps the constants in one place.
 *
 * Algorithm (Acklam):
 *   p_low  = 0.02425
 *   p_high = 1 - p_low
 *   if u < p_low      → q = sqrt(-2 ln u);     rational in q (lower tail)
 *   else if u < p_high → q = u - 0.5; r = q*q; rational in r (central)
 *   else              → q = sqrt(-2 ln(1-u)); rational in q (upper tail)
 *
 * Constants are listed on the Acklam algorithm page.
 *
 * TODO: implement. Until you do, the function returns NaN.
 */
NM_INLINE double nm_inv_normal_cdf(double u) {
    /* TODO: replace with Acklam constants + rational approximations. */
    (void)u;
    return 0.0 / 0.0; /* NaN */
}

#endif /* NM_C_API_INV_NORMAL_CDF_H */
