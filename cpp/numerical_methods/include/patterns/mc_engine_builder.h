#pragma once

// Builder pattern (creational) with build()-time validation.
//
// Why it earns its slot here (vs. just a config struct): construction has
// real invariants — payoff is required, control-variate needs both a
// function AND a mean. build() throws std::invalid_argument when these
// invariants are violated. That's the teaching value: a Builder is more
// than fluent setters.
//
//   auto engine = MonteCarloEngineBuilder()
//                     .with_payoff(call_payoff)
//                     .with_paths(100'000)
//                     .with_antithetic()
//                     .with_control_variate(stock_price_at_T, S0 * exp(r*T))
//                     .build();   // validates, then constructs

#include "mc_estimator.h"
#include "numerical_methods.h"

#include <cstdint>
#include <functional>
#include <memory>
#include <optional>
#include <utility>

namespace nm::patterns {

class MonteCarloEngineBuilder {
public:
    MonteCarloEngineBuilder& with_payoff(func payoff);
    MonteCarloEngineBuilder& with_paths(int n_paths);
    MonteCarloEngineBuilder& with_seed(std::uint64_t seed);
    MonteCarloEngineBuilder& with_antithetic();
    MonteCarloEngineBuilder& with_control_variate(func cv_fn, double cv_mean);

    // Validates invariants:
    //   - payoff must be set                    → std::invalid_argument
    //   - n_paths must be > 0                   → std::invalid_argument
    //   - control_variate either both or neither set
    // Returns the (possibly decorator-wrapped) estimator.
    std::unique_ptr<IMCEstimator> build();  // STUB

private:
    func                                   payoff_;
    int                                    n_paths_        = 0;
    std::uint64_t                          seed_           = 42;
    bool                                   use_antithetic_ = false;
    std::optional<std::pair<func, double>> control_;
};

}  // namespace nm::patterns
