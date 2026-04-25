#include "patterns/solver_factory.h"

#include "patterns/solvers.h"
#include "todo.h"

namespace nm::patterns {

// TODO — switch on `kind` and construct the matching Solver from cfg.
// Validation suggestions:
//   BISECTION/RIDDERS/BRENT/CHANDRUPATLA: require f and a bracket [a,b]
//   NEWTON: require f, df, x0
//   SECANT: require f, x0, x1
//   HALLEY: require f, df, d2f, x0
//   MULLER: require f, x0, x1, x2
// Throw std::invalid_argument if the config is incomplete.
std::unique_ptr<IRootFinder> SolverFactory::create(SolverKind /*kind*/, const SolverConfig& /*cfg*/) {
    NM_TODO("SolverFactory::create");
}

}  // namespace nm::patterns
