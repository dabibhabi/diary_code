#include "random_mc.h"
#include "todo.h"

namespace nm::random_mc {

// TODO: inverse standard normal CDF — Acklam's rational approximation.
//   Tails (u < 0.02425 or u > 0.97575) use a separate rational formula in
//   q = sqrt(-2 ln(u))   (or sqrt(-2 ln(1-u)) for the upper tail).
//   Central region uses a different rational in q = u - 0.5.
//   Constants are listed on the Acklam algorithm page.
// QUANT USE: required for Sobol-driven Monte Carlo (Box-Muller breaks low-
//   discrepancy structure; inverse-CDF preserves it).
double inv_normal_cdf(double /*u*/) { NM_TODO("random_mc::inv_normal_cdf"); }

}  // namespace nm::random_mc
