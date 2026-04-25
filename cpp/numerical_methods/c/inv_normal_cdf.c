/* External-symbol shim for nm_inv_normal_cdf. */
#include "c_api/inv_normal_cdf.h"

double nm_inv_normal_cdf_extern(double u) { return nm_inv_normal_cdf(u); }
