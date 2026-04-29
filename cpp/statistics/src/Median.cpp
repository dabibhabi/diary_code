#include "Median.hpp"

#include <stdexcept>
#include <vector>

namespace stats {

std::string Median::name() const { return "median"; }

double Median::compute(const Dataset& data) const {
    if (data.empty()) throw std::invalid_argument("Median: dataset is empty");

    const std::vector<double> sorted = data.sorted();
    const std::size_t         n      = sorted.size();
    const std::size_t         mid    = n / 2;
    if (n % 2 == 0) return (sorted[mid - 1] + sorted[mid]) / 2.0;
    return sorted[mid];
}

}  // namespace stats
