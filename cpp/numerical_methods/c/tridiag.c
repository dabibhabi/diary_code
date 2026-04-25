/* External-symbol shim for the Thomas tridiagonal solver. */
#include "c_api/tridiag.h"

int nm_thomas_solve_extern(size_t n, double* a, double* b, double* c, double* d, double* x_out) {
    return nm_thomas_solve(n, a, b, c, d, x_out);
}
