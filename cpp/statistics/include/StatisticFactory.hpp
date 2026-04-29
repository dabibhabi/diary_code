#pragma once

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "Statistic.hpp"

namespace stats {

// Registry-based Factory: maps a string key to a constructor for Statistic.
// Singleton accessor; new statistics are registered explicitly at startup or
// via the `register_builtins()` helper.
class StatisticFactory final {
public:
    using Builder = std::function<std::unique_ptr<Statistic>()>;

    StatisticFactory(const StatisticFactory&)            = delete;
    StatisticFactory& operator=(const StatisticFactory&) = delete;

    static StatisticFactory& instance();

    void register_builder(const std::string& key, Builder builder);

    [[nodiscard]] std::unique_ptr<Statistic> create(const std::string& key) const;
    [[nodiscard]] bool                       has(const std::string& key) const;
    [[nodiscard]] std::vector<std::string>   keys() const;

    // Registers Mean, Median, Variance, StandardDeviation, Min, Max, Range, IQR,
    // and the canonical Q1/Q3 quartiles. Idempotent.
    void register_builtins();

private:
    StatisticFactory() = default;
    std::unordered_map<std::string, Builder> builders_;
};

}  // namespace stats
