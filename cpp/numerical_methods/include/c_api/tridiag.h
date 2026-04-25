#ifndef NM_C_API_TRIDIAG_H
#define NM_C_API_TRIDIAG_H

#include "nm_inline.h"

#include <stddef.h>

/* Thomas algorithm for solving a tridiagonal system A x = d:
 *
 *   [ b_0  c_0                    ]   [x_0]   [d_0]
 *   [ a_1  b_1  c_1                ]   [x_1]   [d_1]
 *   [      a_2  b_2  c_2           ] x  ... = ...
 *   [               ...            ]
 *   [               a_{n-1} b_{n-1}]   [x_{n-1}] [d_{n-1}]
 *
 * Why C: tight, allocation-free, predictable codegen. Used by PDE
 * implicit/Crank-Nicolson schemes (one solve per time step) and as a
 * building block for periodic / batched GPU variants.
 *
 * Returns 0 on success, -1 on a zero pivot. Modifies a, b, c, d in place
 * (this is the standard formulation; copy if you need to preserve them).
 *
 * TODO: implement the forward sweep + back substitution.
 */
NM_INLINE int nm_thomas_solve(size_t n, double* NM_RESTRICT a, double* NM_RESTRICT b, double* NM_RESTRICT c,
                              double* NM_RESTRICT d, double* NM_RESTRICT x_out) {
    /* TODO. */
    (void)n;
    (void)a;
    (void)b;
    (void)c;
    (void)d;
    (void)x_out;
    return -1;
}

#endif /* NM_C_API_TRIDIAG_H */
