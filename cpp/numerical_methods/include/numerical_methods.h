#pragma once

#include <functional>
#include <vector>

namespace nm {

using func = std::function<double(double)>;

struct ConvergenceResult {
    double              root;
    int                 iterations;
    bool                converged;
    std::vector<double> errors;
};

// Convergence-rate estimator. Given a sequence of |e_k|, estimates the order
// p in |e_{k+1}| ~ C * |e_k|^p. Returns 0 if too few samples.
double estimate_order(const std::vector<double>& errors);

}  // namespace nm
