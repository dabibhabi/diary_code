// Validates Builder validation: build() must throw on missing payoff or
// non-positive paths. Once the underlying IMCEstimator is also implemented,
// happy-path tests can compare variance with/without decorators.

#include "patterns/mc_engine_builder.h"
#include "todo_helper.h"

#include <stdexcept>

using nm::testing::run_or_todo;
using namespace nm::patterns;

int main() {
    run_or_todo("build() throws when payoff missing", []() {
        bool threw = false;
        try {
            MonteCarloEngineBuilder().with_paths(1000).build();
        } catch (const std::invalid_argument&) { threw = true; }
        NM_CHECK(threw);
    });

    run_or_todo("build() throws when n_paths <= 0", []() {
        bool threw = false;
        try {
            MonteCarloEngineBuilder().with_payoff([](double) { return 0.0; }).with_paths(0).build();
        } catch (const std::invalid_argument&) { threw = true; }
        NM_CHECK(threw);
    });

    return nm::testing::exit_code();
}
