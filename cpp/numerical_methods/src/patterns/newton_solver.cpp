#include "patterns/solvers.h"

#include <cmath>
#include <utility>

namespace nm::patterns {

NewtonSolver::NewtonSolver(func f, func df, double x0) : f_(std::move(f)), df_(std::move(df)), x0_(x0) {}

void NewtonSolver::reset_state() { x_ = x0_; }

// x_{n+1} = x_n - f(x_n) / f'(x_n)
double NewtonSolver::do_step(double& abs_error_out, double& f_value_out) {
    const double fx  = f_(x_);
    const double dfx = df_(x_);
    f_value_out      = fx;
    if (std::abs(dfx) < 1e-14) {
        abs_error_out = 1.0;  // signal stagnation; loop should give up
        return x_;
    }
    const double step = fx / dfx;
    x_ -= step;
    abs_error_out = std::abs(step);
    return x_;
}

}  // namespace nm::patterns
