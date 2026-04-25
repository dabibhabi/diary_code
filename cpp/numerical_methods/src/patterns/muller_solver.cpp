#include "patterns/solvers.h"
#include "todo.h"

#include <utility>

namespace nm::patterns {

MullerSolver::MullerSolver(func f, double x0, double x1, double x2)
    : f_(std::move(f)), x0_init_(x0), x1_init_(x1), x2_init_(x2) {}

void MullerSolver::reset_state() {
    x0_ = x0_init_;
    x1_ = x1_init_;
    x2_ = x2_init_;
}

// TODO — Müller (NR3 §9.5.2). Fit a parabola through (x0, f0), (x1, f1),
// (x2, f2); take its root nearer x2; slide the trio.
double MullerSolver::do_step(double& /*abs_error_out*/, double& /*f_value_out*/) { NM_TODO("MullerSolver::do_step"); }

}  // namespace nm::patterns
