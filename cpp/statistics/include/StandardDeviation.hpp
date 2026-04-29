#pragma once

#include "Statistic.hpp"
#include "Variance.hpp"

namespace stats {

// Composition: delegates to Variance and applies sqrt.
class StandardDeviation final : public Statistic {
public:
    explicit StandardDeviation(bool sample = true);

    [[nodiscard]] std::string name() const override;
    [[nodiscard]] double      compute(const Dataset& data) const override;

private:
    Variance variance_;
};

}  // namespace stats
