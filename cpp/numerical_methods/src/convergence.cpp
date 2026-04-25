#include "numerical_methods.h"

#include <cmath>

namespace nm {

// Estimate the order of convergence p from a sequence of errors e_k.
// Uses the relation e_{k+1} ~ C * e_k^p, so
//     p ~ log(e_{k+1}/e_k) / log(e_k/e_{k-1}).
// We average over the last few iterations for stability.
double estimate_order(const std::vector<double>& errors) {
    if (errors.size() < 4) return 0.0;
    double sum   = 0.0;
    int    count = 0;
    for (size_t k = errors.size() - 1; k >= 2 && count < 5; --k) {
        const double a = errors[k];
        const double b = errors[k - 1];
        const double c = errors[k - 2];
        if (a <= 0 || b <= 0 || c <= 0) continue;
        const double num = std::log(a / b);
        const double den = std::log(b / c);
        if (std::abs(den) < 1e-14) continue;
        sum += num / den;
        ++count;
        if (k == 2) break;
    }
    return count == 0 ? 0.0 : sum / count;
}

}  // namespace nm
