#pragma once

#include "Statistic.hpp"

namespace stats {

class Min final : public Statistic {
public:
    [[nodiscard]] std::string name() const override;
    [[nodiscard]] double      compute(const Dataset& data) const override;
};

class Max final : public Statistic {
public:
    [[nodiscard]] std::string name() const override;
    [[nodiscard]] double      compute(const Dataset& data) const override;
};

}  // namespace stats
