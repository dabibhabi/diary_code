#include "StandardDeviation.hpp"

#include <cmath>

namespace stats {

StandardDeviation::StandardDeviation(bool sample) : variance_(sample) {}

std::string StandardDeviation::name() const { return "stddev"; }

double StandardDeviation::compute(const Dataset& data) const { return std::sqrt(variance_.compute(data)); }

}  // namespace stats
