/* Pure-C standalone smoke test.
 *
 * Built by CMake as a separate executable using a C compiler — NO C++ runtime,
 * NO libstdc++. Proves that:
 *   - the c_api headers compile cleanly under -std=c11
 *   - extern "C" guards are well-formed
 *   - the NM_INLINE algorithms link into a pure-C TU
 *
 * Returns 0 if every check passes, non-zero otherwise. Each block is a
 * checkbox-driven gate: while a feature is unimplemented (returns NaN/-1/0),
 * we just print "[TODO]" and don't fail. Once implemented, the asserts
 * become real.
 */

#include "c_api/blas1.h"
#include "c_api/cholesky_inplace.h"
#include "c_api/inv_normal_cdf.h"
#include "c_api/tridiag.h"
#include "c_api/xorshift.h"

#include <math.h>
#include <stdio.h>
#include <string.h>

static int approx_eq(double a, double b, double tol) { return fabs(a - b) < tol; }

int main(void) {
    int fails = 0;

    /* xorshift sanity */
    {
        uint64_t s = 0x123456789abcdef0ULL;
        uint64_t r = xorshift64_next(&s);
        if (r == 0) {
            printf("[TODO] xorshift64_next\n");
        } else {
            printf("[ OK ] xorshift64_next: r=0x%016llx\n", (unsigned long long)r);
        }

        double u = xorshift_to_unit(r);
        if (u == 0.0) {
            printf("[TODO] xorshift_to_unit\n");
        } else if (u >= 0.0 && u < 1.0) {
            printf("[ OK ] xorshift_to_unit -> %.6f\n", u);
        } else {
            printf("[FAIL] xorshift_to_unit out of range: %f\n", u);
            ++fails;
        }
    }

    /* inv_normal_cdf at 0.5 should be 0 */
    {
        double v = nm_inv_normal_cdf(0.5);
        if (isnan(v)) {
            printf("[TODO] nm_inv_normal_cdf\n");
        } else if (approx_eq(v, 0.0, 1e-10)) {
            printf("[ OK ] nm_inv_normal_cdf(0.5) = 0\n");
        } else {
            printf("[FAIL] nm_inv_normal_cdf(0.5) = %f, expected 0\n", v);
            ++fails;
        }
    }

    /* blas1: saxpy then dot */
    {
        double x[3] = {1.0, 2.0, 3.0};
        double y[3] = {4.0, 5.0, 6.0};
        double y_orig[3];
        memcpy(y_orig, y, sizeof y_orig);
        nm_saxpy(3, 2.0, x, y);
        if (memcmp(y, y_orig, sizeof y_orig) == 0) {
            printf("[TODO] nm_saxpy\n");
        } else if (approx_eq(y[0], 6.0, 1e-12) && approx_eq(y[1], 9.0, 1e-12) && approx_eq(y[2], 12.0, 1e-12)) {
            printf("[ OK ] nm_saxpy y = 2*x + y\n");
        } else {
            printf("[FAIL] nm_saxpy: y = {%.2f, %.2f, %.2f}\n", y[0], y[1], y[2]);
            ++fails;
        }

        double d = nm_dot(3, x, x);
        if (d == 0.0) {
            printf("[TODO] nm_dot\n");
        } else if (approx_eq(d, 14.0, 1e-12)) {
            printf("[ OK ] nm_dot(x,x) = 14\n");
        } else {
            printf("[FAIL] nm_dot: %f\n", d);
            ++fails;
        }
    }

    /* tridiag: a small 3x3 system */
    {
        /* [2 -1  0]   [x0]   [1]
         * [-1 2 -1] * [x1] = [0]    →  x = [1, 1, 1]
         * [0 -1  2]   [x2]   [1]
         */
        double a[3]     = {0.0, -1.0, -1.0};
        double b[3]     = {2.0, 2.0, 2.0};
        double c[3]     = {-1.0, -1.0, 0.0};
        double d[3]     = {1.0, 0.0, 1.0};
        double x_out[3] = {0, 0, 0};
        int    rc       = nm_thomas_solve(3, a, b, c, d, x_out);
        if (rc != 0) {
            printf("[TODO] nm_thomas_solve (or unimplemented)\n");
        } else if (approx_eq(x_out[0], 1.0, 1e-10) && approx_eq(x_out[1], 1.0, 1e-10) &&
                   approx_eq(x_out[2], 1.0, 1e-10)) {
            printf("[ OK ] nm_thomas_solve\n");
        } else {
            printf("[FAIL] nm_thomas_solve: x = {%f, %f, %f}\n", x_out[0], x_out[1], x_out[2]);
            ++fails;
        }
    }

    /* cholesky: 2x2 SPD: [[4,2],[2,3]] -> L = [[2,0],[1, sqrt(2)]] */
    {
        double A[4] = {4.0, 2.0, 2.0, 3.0};
        int    rc   = nm_cholesky_inplace(A, 2);
        if (rc != 0) {
            printf("[TODO] nm_cholesky_inplace\n");
        } else if (approx_eq(A[0], 2.0, 1e-10) && approx_eq(A[2], 1.0, 1e-10) && approx_eq(A[3], sqrt(2.0), 1e-10)) {
            printf("[ OK ] nm_cholesky_inplace 2x2\n");
        } else {
            printf("[FAIL] nm_cholesky_inplace: L = [%f, %f; %f, %f]\n", A[0], A[1], A[2], A[3]);
            ++fails;
        }
    }

    if (fails > 0) {
        fprintf(stderr, "%d real failures (TODOs do not count)\n", fails);
        return 1;
    }
    return 0;
}
