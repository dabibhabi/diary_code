#pragma once

// Decorator pattern (structural) — variance-reduction wrappers stack onto
// any IMCEstimator without touching its internals.
//
//   auto base    = std::make_unique<MyMCEstimator>(...);
//   auto with_av = std::make_unique<AntitheticDecorator>(std::move(base));
//   auto with_cv = std::make_unique<ControlVariateDecorator>(std::move(with_av), cv_fn, cv_mean);
//   auto result  = with_cv->estimate(100000, seed);
//
// Why it fits: variance reduction is an additive concern. Decorator avoids
// `if (use_av) { ... } else if (use_cv) { ... }` branching in the engine.

#include "mc_estimator.h"
#include "numerical_methods.h"

#include <memory>

namespace nm::patterns {

// Pairs (Z, -Z) over n_paths/2 samples. Halves variance for monotone payoffs.
class AntitheticDecorator : public IMCEstimator {
public:
    explicit AntitheticDecorator(std::unique_ptr<IMCEstimator> base);
    MCResult estimate(int n_paths, std::uint64_t seed) const override;  // STUB

private:
    std::unique_ptr<IMCEstimator> base_;
};

// Subtracts a correlated, known-mean estimator: X' = X - β̂(Y - μ_Y).
// β̂ estimated from the sample.
class ControlVariateDecorator : public IMCEstimator {
public:
    ControlVariateDecorator(std::unique_ptr<IMCEstimator> base, func control_fn, double control_mean);
    MCResult estimate(int n_paths, std::uint64_t seed) const override;  // STUB

private:
    std::unique_ptr<IMCEstimator> base_;
    func                          control_fn_;
    double                        control_mean_;
};

}  // namespace nm::patterns
