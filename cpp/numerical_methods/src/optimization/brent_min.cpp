#include "optimization.h"
#include "todo.h"

namespace nm::optimization {

// TODO: Brent's 1D minimization.
//   Combine golden-section (safe) with parabolic interpolation through the
//   three best points (fast, when usable). Same spirit as Brent's root-finder.
// References: NR3 §10.3 (Algorithm BRENT).
double brent_min(func /*f*/, double /*a*/, double /*b*/, double /*tol*/, int /*max_iter*/) {
    NM_TODO("optimization::brent_min");
}

}  // namespace nm::optimization
