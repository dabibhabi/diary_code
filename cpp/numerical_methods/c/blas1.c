/* External-symbol shims for the BLAS-1 helpers. The shareable bodies live
 * in c_api/blas1.h. */
#include "c_api/blas1.h"

void   nm_saxpy_extern(size_t n, double a, const double* x, double* y) { nm_saxpy(n, a, x, y); }
double nm_dot_extern(size_t n, const double* x, const double* y) { return nm_dot(n, x, y); }
void   nm_scale_extern(size_t n, double a, double* x) { nm_scale(n, a, x); }
