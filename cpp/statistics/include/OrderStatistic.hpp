#pragma once

#include <cstddef>

#include "Statistic.hpp"

namespace stats {

class OrderStatistic final : public Statistic {
public:
    explicit OrderStatistic(std::size_t k);

    [[nodiscard]] std::string name() const override;
    [[nodiscard]] double      compute(const Dataset& data) const override;

private:
    std::size_t k_;
};

class Quantile final : public Statistic {
public:
    explicit Quantile(double q);

    [[nodiscard]] std::string name() const override;
    [[nodiscard]] double      compute(const Dataset& data) const override;

private:
    double q_;
};

class Range final : public Statistic {
public:
    [[nodiscard]] std::string name() const override;
    [[nodiscard]] double      compute(const Dataset& data) const override;
};

class IQR final : public Statistic {
public:
    [[nodiscard]] std::string name() const override;
    [[nodiscard]] double      compute(const Dataset& data) const override;
};

}  // namespace stats
