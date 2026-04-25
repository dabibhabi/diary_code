// Calls each c_api/*.h function from C++ to validate that the headers are
// extern "C"-clean and that the NM_INLINE algorithm bodies behave the same
// when called from a C++ TU as from c/test_c_api.c.
//
// The C API uses return-code error handling (no exceptions). To mirror the
// run_or_todo pattern, this file probes each function for its stub sentinel
// (NaN / rc=-1 / unchanged buffer) and converts that into a thrown
// nm::not_implemented so the test reports [TODO] instead of [FAIL].

#include "c_api/blas1.h"
#include "c_api/cholesky_inplace.h"
#include "c_api/inv_normal_cdf.h"
#include "c_api/tridiag.h"
#include "c_api/xorshift.h"
#include "todo.h"
#include "todo_helper.h"

#include <cmath>
#include <cstdint>
#include <cstring>

using nm::testing::run_or_todo;

#define NM_TODO_IF(cond, name) \
    if (cond) NM_TODO(name)

int main() {
    run_or_todo("xorshift64_next mutates state", []() {
        std::uint64_t s = 0x123456789abcdef0ULL;
        std::uint64_t r = xorshift64_next(&s);
        NM_TODO_IF(r == 0, "xorshift64_next");
        NM_CHECK(s == r);
    });

    run_or_todo("xorshift_to_unit in [0, 1)", []() {
        double u = xorshift_to_unit(0xdeadbeefcafebabeULL);
        NM_TODO_IF(u == 0.0, "xorshift_to_unit");
        NM_CHECK(u >= 0.0);
        NM_CHECK(u < 1.0);
    });

    run_or_todo("inv_normal_cdf central + tail", []() {
        double v = nm_inv_normal_cdf(0.5);
        NM_TODO_IF(std::isnan(v), "nm_inv_normal_cdf");
        NM_CHECK_NEAR(v, 0.0, 1e-10);
        NM_CHECK_NEAR(nm_inv_normal_cdf(0.8413447), 1.0, 1e-5);
    });

    run_or_todo("nm_saxpy / nm_dot", []() {
        double x[3] = {1, 2, 3}, y[3] = {4, 5, 6};
        double y_orig[3] = {4, 5, 6};
        nm_saxpy(3, 2.0, x, y);
        NM_TODO_IF(std::memcmp(y, y_orig, sizeof y_orig) == 0, "nm_saxpy");
        NM_CHECK_NEAR(y[0], 6.0, 1e-12);
        NM_CHECK_NEAR(y[2], 12.0, 1e-12);

        double d = nm_dot(3, x, x);
        NM_TODO_IF(d == 0.0, "nm_dot");
        NM_CHECK_NEAR(d, 14.0, 1e-12);
    });

    run_or_todo("cholesky_inplace 2x2 SPD", []() {
        double A[4] = {4.0, 2.0, 2.0, 3.0};
        int    rc   = nm_cholesky_inplace(A, 2);
        NM_TODO_IF(rc == -1 && A[0] == 4.0, "nm_cholesky_inplace");
        NM_CHECK(rc == 0);
        NM_CHECK_NEAR(A[0], 2.0, 1e-10);
        NM_CHECK_NEAR(A[2], 1.0, 1e-10);
        NM_CHECK_NEAR(A[3], std::sqrt(2.0), 1e-10);
    });

    run_or_todo("Thomas tridiag solver", []() {
        double a[3]  = {0.0, -1.0, -1.0};
        double b[3]  = {2.0, 2.0, 2.0};
        double c[3]  = {-1.0, -1.0, 0.0};
        double d[3]  = {1.0, 0.0, 1.0};
        double xo[3] = {0, 0, 0};
        int    rc    = nm_thomas_solve(3, a, b, c, d, xo);
        NM_TODO_IF(rc == -1, "nm_thomas_solve");
        NM_CHECK(rc == 0);
        NM_CHECK_NEAR(xo[0], 1.0, 1e-10);
        NM_CHECK_NEAR(xo[2], 1.0, 1e-10);
    });

    return nm::testing::exit_code();
}
