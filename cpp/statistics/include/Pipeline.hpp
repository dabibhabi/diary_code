#pragma once

#include <memory>
#include <string>
#include <vector>

#include "Dataset.hpp"
#include "Statistic.hpp"

namespace stats {

struct NamedResult {
    std::string name;
    double      value;
};

// Composite: aggregates Statistic strategies and runs them together.
// Holds ownership of its strategies. Fluent interface for setup.
class StatisticsPipeline final {
public:
    StatisticsPipeline& add(std::unique_ptr<Statistic> stat);

    template <typename T, typename... Args>
    StatisticsPipeline& emplace(Args&&... args) {
        return add(std::make_unique<T>(std::forward<Args>(args)...));
    }

    [[nodiscard]] std::vector<NamedResult> run(const Dataset& data) const;
    [[nodiscard]] std::size_t              size() const noexcept;

private:
    std::vector<std::unique_ptr<Statistic>> stats_;
};

}  // namespace stats
