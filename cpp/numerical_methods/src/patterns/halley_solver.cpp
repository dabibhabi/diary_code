#include "patterns/solvers.h"
#include "todo.h"

#include <utility>

namespace nm::patterns {

HalleySolver::HalleySolver(func f, func df, func d2f, double x0)
    : f_(std::move(f)), df_(std::move(df)), d2f_(std::move(d2f)), x0_(x0) {}

void HalleySolver::reset_state() { x_ = x0_; }

// TODO — Halley's update: x_{n+1} = x_n - 2 f f' / (2 (f')^2 - f f'').
// Cubic convergence near a simple root.
double HalleySolver::do_step(double& /*abs_error_out*/, double& /*f_value_out*/) { NM_TODO("HalleySolver::do_step"); }

}  // namespace nm::patterns
