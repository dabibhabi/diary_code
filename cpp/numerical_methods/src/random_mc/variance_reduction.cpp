#include "random_mc.h"
#include "todo.h"

namespace nm::random_mc {

// TODO: Monte Carlo with control variate.
//   1. Run estimator(seed) for n_paths to collect (X_i, Y_i) where E[Y]=μ_Y.
//   2. Estimate β̂ = Cov(X,Y) / Var(Y).
//   3. Adjusted estimator: X_i - β̂ (Y_i - μ_Y). Mean and stderr of that.
//   For antithetic variates, run estimator twice with (seed) and (~seed) and
//   average — typically halves variance for a monotone payoff.
// References: Glasserman ch. 4.
MCResult monte_carlo_with_control(std::function<std::pair<double, double>(uint64_t)> /*estimator*/,
                                  double /*control_mean*/, int /*n_paths*/, uint64_t /*seed*/) {
    NM_TODO("random_mc::monte_carlo_with_control");
}

}  // namespace nm::random_mc
