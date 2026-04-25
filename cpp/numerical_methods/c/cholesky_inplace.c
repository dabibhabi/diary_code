/* External-symbol shim for in-place Cholesky. */
#include "c_api/cholesky_inplace.h"

int nm_cholesky_inplace_extern(double* A, int n) { return nm_cholesky_inplace(A, n); }
