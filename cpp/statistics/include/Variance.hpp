#pragma once

#include "Statistic.hpp"

namespace stats {

class Variance final : public Statistic {
public:
    explicit Variance(bool sample = true);

    [[nodiscard]] std::string name() const override;
    [[nodiscard]] double      compute(const Dataset& data) const override;

private:
    bool sample_;
};

}  // namespace stats
