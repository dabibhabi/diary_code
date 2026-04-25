#include "patterns/solvers.h"
#include "todo.h"

#include <utility>

namespace nm::patterns {

ChandrupatlaSolver::ChandrupatlaSolver(func f, double a, double b) : f_(std::move(f)), a0_(a), b0_(b) {}

void ChandrupatlaSolver::reset_state() {
    a_ = a0_;
    b_ = b0_;
}

// TODO — Chandrupatla 1997. Same shape as Brent, tighter acceptance test
// (xi-vs-phi). Search "Chandrupatla algorithm pseudocode" for derivations.
double ChandrupatlaSolver::do_step(double& /*abs_error_out*/, double& /*f_value_out*/) {
    NM_TODO("ChandrupatlaSolver::do_step");
}

}  // namespace nm::patterns
