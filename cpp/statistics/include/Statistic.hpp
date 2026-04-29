#pragma once

#include <string>

#include "Dataset.hpp"

namespace stats {

// Strategy: defines an interchangeable algorithm that maps a Dataset to a scalar.
class Statistic {
public:
    virtual ~Statistic() = default;

    [[nodiscard]] virtual std::string name() const                       = 0;
    [[nodiscard]] virtual double      compute(const Dataset& data) const = 0;

protected:
    Statistic()                                = default;
    Statistic(const Statistic&)                = default;
    Statistic(Statistic&&) noexcept            = default;
    Statistic& operator=(const Statistic&)     = default;
    Statistic& operator=(Statistic&&) noexcept = default;
};

}  // namespace stats
