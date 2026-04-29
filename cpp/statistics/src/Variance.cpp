#include "Variance.hpp"

#include <stdexcept>

#include "Mean.hpp"

namespace stats {

Variance::Variance(bool sample) : sample_(sample) {}

std::string Variance::name() const { return sample_ ? "variance(sample)" : "variance(population)"; }

double Variance::compute(const Dataset& data) const {
    const std::size_t n = data.size();
    if (n == 0) throw std::invalid_argument("Variance: dataset is empty");
    if (sample_ && n < 2) throw std::invalid_argument("Variance(sample): need at least 2 values");

    const double mu     = Mean{}.compute(data);
    double       sum_sq = 0.0;
    for (double x : data.values()) {
        const double d = x - mu;
        sum_sq += d * d;
    }
    const double denom = sample_ ? static_cast<double>(n - 1) : static_cast<double>(n);
    return sum_sq / denom;
}

}  // namespace stats
