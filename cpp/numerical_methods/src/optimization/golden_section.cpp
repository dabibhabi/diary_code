#include "optimization.h"
#include "todo.h"

namespace nm::optimization {

// TODO: golden-section search.
//   φ = (1 + √5)/2; resphi = 2 - φ.
//   Place x1 = a + resphi*(b-a), x2 = b - resphi*(b-a).
//   Compare f(x1), f(x2); shrink bracket on the higher side. Reuse one
//   evaluation per iteration (the eponymous "trick").
double golden_section(func /*f*/, double /*a*/, double /*b*/, double /*tol*/) {
    NM_TODO("optimization::golden_section");
}

}  // namespace nm::optimization
