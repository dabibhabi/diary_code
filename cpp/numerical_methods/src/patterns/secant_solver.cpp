#include "patterns/solvers.h"

#include <cmath>
#include <utility>

namespace nm::patterns {

SecantSolver::SecantSolver(func f, double x0, double x1) : f_(std::move(f)), x0_init_(x0), x1_init_(x1) {}

void SecantSolver::reset_state() {
    x0_ = x0_init_;
    x1_ = x1_init_;
    f0_ = f_(x0_);
    f1_ = f_(x1_);
}

double SecantSolver::do_step(double& abs_error_out, double& f_value_out) {
    const double denom = f1_ - f0_;
    if (std::abs(denom) < 1e-14) {
        abs_error_out = 1.0;
        f_value_out   = f1_;
        return x1_;
    }
    const double x2 = x1_ - f1_ * (x1_ - x0_) / denom;
    abs_error_out   = std::abs(x2 - x1_);
    x0_             = x1_;
    f0_             = f1_;
    x1_             = x2;
    f1_             = f_(x1_);
    f_value_out     = f1_;
    return x1_;
}

}  // namespace nm::patterns
