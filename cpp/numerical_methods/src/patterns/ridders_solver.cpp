#include "patterns/solvers.h"
#include "todo.h"

#include <utility>

namespace nm::patterns {

RiddersSolver::RiddersSolver(func f, double a, double b) : f_(std::move(f)), a0_(a), b0_(b) {}

void RiddersSolver::reset_state() {
    a_ = a0_;
    b_ = b0_;
    x_ = 0.5 * (a_ + b_);
}

// TODO — Ridders' method (NR3 §9.2.1).
//   m  = (a + b) / 2
//   fm = f(m)
//   s  = sqrt(fm*fm - f(a)*f(b))
//   x_new = m + (m - a) * sign(f(a) - f(b)) * fm / s
//   replace whichever endpoint preserves the bracket around x_new.
double RiddersSolver::do_step(double& /*abs_error_out*/, double& /*f_value_out*/) { NM_TODO("RiddersSolver::do_step"); }

}  // namespace nm::patterns
