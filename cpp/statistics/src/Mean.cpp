#include "Mean.hpp"

#include <numeric>
#include <stdexcept>

namespace stats {

std::string Mean::name() const { return "mean"; }

double Mean::compute(const Dataset& data) const {
    if (data.empty()) throw std::invalid_argument("Mean: dataset is empty");
    const auto& v = data.values();
    return std::accumulate(v.begin(), v.end(), 0.0) / static_cast<double>(v.size());
}

}  // namespace stats
