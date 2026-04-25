#include "patterns/mc_engine_builder.h"

#include "patterns/mc_decorators.h"
#include "todo.h"

#include <stdexcept>
#include <utility>

namespace nm::patterns {

MonteCarloEngineBuilder& MonteCarloEngineBuilder::with_payoff(func payoff) {
    payoff_ = std::move(payoff);
    return *this;
}

MonteCarloEngineBuilder& MonteCarloEngineBuilder::with_paths(int n_paths) {
    n_paths_ = n_paths;
    return *this;
}

MonteCarloEngineBuilder& MonteCarloEngineBuilder::with_seed(std::uint64_t seed) {
    seed_ = seed;
    return *this;
}

MonteCarloEngineBuilder& MonteCarloEngineBuilder::with_antithetic() {
    use_antithetic_ = true;
    return *this;
}

MonteCarloEngineBuilder& MonteCarloEngineBuilder::with_control_variate(func cv_fn, double cv_mean) {
    control_ = std::make_pair(std::move(cv_fn), cv_mean);
    return *this;
}

// TODO — validate then construct.
//   if (!payoff_)              throw std::invalid_argument("payoff required");
//   if (n_paths_ <= 0)         throw std::invalid_argument("n_paths must be > 0");
//   construct a base estimator (you'll need to implement a concrete IMCEstimator
//   such as a GBM-path engine that uses payoff_, seed_, and n_paths_), then
//   wrap it in AntitheticDecorator if use_antithetic_, and ControlVariateDecorator
//   if control_ is set.
std::unique_ptr<IMCEstimator> MonteCarloEngineBuilder::build() { NM_TODO("MonteCarloEngineBuilder::build"); }

}  // namespace nm::patterns
