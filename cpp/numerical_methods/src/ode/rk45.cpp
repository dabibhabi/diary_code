#include "ode.h"
#include "todo.h"

namespace nm::ode {

// TODO: Dormand-Prince adaptive RK45.
//   1. Compute the 6 stages (look up the RKDP Butcher tableau).
//   2. Form 5th-order solution y5 and 4th-order solution y4.
//   3. err = ||y5 - y4||; if err < tol → accept y5, grow h; else shrink h.
//   4. h_new = h * 0.9 * (tol/err)^{1/5}, clamped.
// References: Wikipedia "Dormand-Prince_method"; NR3 §17.2.
Trajectory rk45(RHS /*f*/, double /*t0*/, State /*y0*/, double /*t_end*/, double /*h_init*/, double /*tol*/) {
    NM_TODO("ode::rk45");
}

}  // namespace nm::ode
