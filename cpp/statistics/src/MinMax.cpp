#include "MinMax.hpp"

#include <algorithm>
#include <stdexcept>

namespace stats {

std::string Min::name() const { return "min"; }

double Min::compute(const Dataset& data) const {
    if (data.empty()) throw std::invalid_argument("Min: dataset is empty");
    const auto& v = data.values();
    return *std::min_element(v.begin(), v.end());
}

std::string Max::name() const { return "max"; }

double Max::compute(const Dataset& data) const {
    if (data.empty()) throw std::invalid_argument("Max: dataset is empty");
    const auto& v = data.values();
    return *std::max_element(v.begin(), v.end());
}

}  // namespace stats
