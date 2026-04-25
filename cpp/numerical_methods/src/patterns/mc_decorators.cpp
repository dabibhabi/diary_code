#include "patterns/mc_decorators.h"

#include "todo.h"

#include <utility>

namespace nm::patterns {

AntitheticDecorator::AntitheticDecorator(std::unique_ptr<IMCEstimator> base) : base_(std::move(base)) {}

// TODO — call base_->estimate(n_paths/2, seed) twice, once with `seed`
// and once with antithetic samples (e.g. seed | 0x1ULL << 63), then
// average. Reduces variance for monotone payoffs.
MCResult AntitheticDecorator::estimate(int /*n_paths*/, std::uint64_t /*seed*/) const {
    NM_TODO("AntitheticDecorator::estimate");
}

ControlVariateDecorator::ControlVariateDecorator(std::unique_ptr<IMCEstimator> base, func cv_fn, double cv_mean)
    : base_(std::move(base)), control_fn_(std::move(cv_fn)), control_mean_(cv_mean) {}

// TODO — Glasserman ch. 4. Run base_->estimate, collect (X_i, Y_i),
// estimate beta = Cov(X,Y) / Var(Y), report mean of (X - beta*(Y - mu_Y)).
MCResult ControlVariateDecorator::estimate(int /*n_paths*/, std::uint64_t /*seed*/) const {
    NM_TODO("ControlVariateDecorator::estimate");
}

}  // namespace nm::patterns
