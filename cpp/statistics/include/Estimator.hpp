#pragma once

#include <string>
#include <vector>

#include "Dataset.hpp"

namespace stats {

struct Estimate {
    std::string parameter;
    double      value;
};

// Strategy: produces one or more parameter estimates from a sample.
class Estimator {
public:
    virtual ~Estimator() = default;

    [[nodiscard]] virtual std::string           name() const                        = 0;
    [[nodiscard]] virtual std::vector<Estimate> estimate(const Dataset& data) const = 0;

protected:
    Estimator()                                = default;
    Estimator(const Estimator&)                = default;
    Estimator(Estimator&&) noexcept            = default;
    Estimator& operator=(const Estimator&)     = default;
    Estimator& operator=(Estimator&&) noexcept = default;
};

}  // namespace stats
