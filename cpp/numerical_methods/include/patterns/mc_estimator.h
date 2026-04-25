#pragma once

// Monte Carlo Estimator interface — Strategy + Decorator.
//
// IMCEstimator is the base; AntitheticDecorator and ControlVariateDecorator
// (in mc_decorators.h) wrap it to add variance reduction without modifying
// the underlying estimator.

#include "random_mc.h"

#include <cstdint>
#include <functional>
#include <memory>

namespace nm::patterns {

using nm::random_mc::MCResult;

class IMCEstimator {
public:
    virtual ~IMCEstimator() = default;

    // Run n_paths simulations starting from `seed`. Returns mean + stderr.
    virtual MCResult estimate(int n_paths, std::uint64_t seed) const = 0;
};

}  // namespace nm::patterns
