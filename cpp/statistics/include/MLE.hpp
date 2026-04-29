#pragma once

#include "Estimator.hpp"

namespace stats {

class NormalMLE final : public Estimator {
public:
    [[nodiscard]] std::string           name() const override;
    [[nodiscard]] std::vector<Estimate> estimate(const Dataset& data) const override;
};

class ExponentialMLE final : public Estimator {
public:
    [[nodiscard]] std::string           name() const override;
    [[nodiscard]] std::vector<Estimate> estimate(const Dataset& data) const override;
};

class BernoulliMLE final : public Estimator {
public:
    [[nodiscard]] std::string           name() const override;
    [[nodiscard]] std::vector<Estimate> estimate(const Dataset& data) const override;
};

class PoissonMLE final : public Estimator {
public:
    [[nodiscard]] std::string           name() const override;
    [[nodiscard]] std::vector<Estimate> estimate(const Dataset& data) const override;
};

}  // namespace stats
