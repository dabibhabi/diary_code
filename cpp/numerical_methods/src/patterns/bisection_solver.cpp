#include "patterns/solvers.h"

#include <cmath>
#include <utility>

namespace nm::patterns {

BisectionSolver::BisectionSolver(func f, double a, double b) : f_(std::move(f)), a0_(a), b0_(b) {}

void BisectionSolver::reset_state() {
    a_   = a0_;
    b_   = b0_;
    fa_  = f_(a_);
    fb_  = f_(b_);
    mid_ = a_;
}

// One bisection step: midpoint, evaluate, choose half preserving the sign change.
double BisectionSolver::do_step(double& abs_error_out, double& f_value_out) {
    mid_            = 0.5 * (a_ + b_);
    const double fm = f_(mid_);
    abs_error_out   = 0.5 * std::abs(b_ - a_);
    f_value_out     = fm;
    if (fa_ * fm < 0) {
        b_  = mid_;
        fb_ = fm;
    } else {
        a_  = mid_;
        fa_ = fm;
    }
    return mid_;
}

}  // namespace nm::patterns
