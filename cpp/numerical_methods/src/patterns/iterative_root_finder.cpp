#include "patterns/iterative_root_finder.h"

#include "todo.h"

namespace nm::patterns {

bool IRootFinder::notify(const IterationEvent& e) {
    bool stop = false;
    for (auto& obs : observers_) {
        obs->on_iteration(e);
        if (obs->should_stop(e)) stop = true;
    }
    return stop;
}

// TODO — the Template Method skeleton.
//
// Pseudocode (the user implements this):
//   res = ConvergenceResult{0.0, 0, false, {}};
//   reset_state();
//   for (int k = 0; k < max_iter; ++k) {
//       double abs_err = 0, fx = 0;
//       const double x = do_step(abs_err, fx);
//       res.errors.push_back(abs_err);
//       ++res.iterations;
//       const bool stop = notify({k, x, abs_err, fx});
//       if (is_converged(abs_err, tol) || stop) {
//           res.root      = x;
//           res.converged = is_converged(abs_err, tol);
//           return res;
//       }
//   }
//   res.root = current_x();
//   return res;
ConvergenceResult IterativeRootFinder::solve(double /*tol*/, int /*max_iter*/) {
    NM_TODO("IterativeRootFinder::solve");
}

}  // namespace nm::patterns
