#include "optimization.h"
#include "todo.h"

namespace nm::optimization {

// TODO: Nelder-Mead simplex.
//   Start with simplex of n+1 vertices; each iteration:
//     1. order: best, ..., second-worst, worst
//     2. reflect worst through centroid; if better than second-worst → accept
//     3. expand if reflection beats the best
//     4. contract toward centroid if reflection is worse than second-worst
//     5. shrink the whole simplex toward best if all else fails
// References: Wikipedia "Nelder-Mead_method"; NR3 §10.5.
OptResult nelder_mead(ObjectiveND /*f*/, linalg::Vector /*x0*/, double /*tol*/, int /*max_iter*/) {
    NM_TODO("optimization::nelder_mead");
}

}  // namespace nm::optimization
