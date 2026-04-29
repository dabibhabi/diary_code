#include "OrderStatistic.hpp"

#include <cmath>
#include <stdexcept>
#include <string>
#include <vector>

namespace stats {

OrderStatistic::OrderStatistic(std::size_t k) : k_(k) {
    if (k_ == 0) throw std::invalid_argument("OrderStatistic: k must be 1-indexed (>=1)");
}

std::string OrderStatistic::name() const { return "order(" + std::to_string(k_) + ")"; }

double OrderStatistic::compute(const Dataset& data) const {
    if (k_ > data.size()) throw std::invalid_argument("OrderStatistic: k exceeds dataset size");
    return data.sorted()[k_ - 1];
}

Quantile::Quantile(double q) : q_(q) {
    if (q_ < 0.0 || q_ > 1.0) throw std::invalid_argument("Quantile: q must be in [0, 1]");
}

std::string Quantile::name() const { return "quantile(" + std::to_string(q_) + ")"; }

double Quantile::compute(const Dataset& data) const {
    if (data.empty()) throw std::invalid_argument("Quantile: dataset is empty");
    const std::vector<double> sorted = data.sorted();
    const std::size_t         n      = sorted.size();
    if (n == 1) return sorted[0];

    // R's "type 7" definition: linear interpolation between order statistics.
    const double      h  = (static_cast<double>(n) - 1.0) * q_;
    const std::size_t lo = static_cast<std::size_t>(std::floor(h));
    const std::size_t hi = static_cast<std::size_t>(std::ceil(h));
    if (lo == hi) return sorted[lo];
    const double frac = h - static_cast<double>(lo);
    return sorted[lo] + frac * (sorted[hi] - sorted[lo]);
}

std::string Range::name() const { return "range"; }

double Range::compute(const Dataset& data) const {
    if (data.empty()) throw std::invalid_argument("Range: dataset is empty");
    const std::vector<double> sorted = data.sorted();
    return sorted.back() - sorted.front();
}

std::string IQR::name() const { return "iqr"; }

double IQR::compute(const Dataset& data) const { return Quantile{0.75}.compute(data) - Quantile{0.25}.compute(data); }

}  // namespace stats
