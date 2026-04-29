#include "StatisticFactory.hpp"

#include <stdexcept>
#include <utility>

#include "Mean.hpp"
#include "Median.hpp"
#include "MinMax.hpp"
#include "OrderStatistic.hpp"
#include "StandardDeviation.hpp"
#include "Variance.hpp"

namespace stats {

StatisticFactory& StatisticFactory::instance() {
    static StatisticFactory factory;
    return factory;
}

void StatisticFactory::register_builder(const std::string& key, Builder builder) {
    if (!builder) throw std::invalid_argument("StatisticFactory::register_builder: null builder");
    builders_[key] = std::move(builder);
}

std::unique_ptr<Statistic> StatisticFactory::create(const std::string& key) const {
    auto it = builders_.find(key);
    if (it == builders_.end()) throw std::invalid_argument("StatisticFactory::create: unknown key '" + key + "'");
    return it->second();
}

bool StatisticFactory::has(const std::string& key) const { return builders_.find(key) != builders_.end(); }

std::vector<std::string> StatisticFactory::keys() const {
    std::vector<std::string> out;
    out.reserve(builders_.size());
    for (const auto& [k, _] : builders_)
        out.push_back(k);
    return out;
}

void StatisticFactory::register_builtins() {
    register_builder("mean", [] { return std::make_unique<Mean>(); });
    register_builder("median", [] { return std::make_unique<Median>(); });
    register_builder("var", [] { return std::make_unique<Variance>(true); });
    register_builder("stddev", [] { return std::make_unique<StandardDeviation>(true); });
    register_builder("min", [] { return std::make_unique<Min>(); });
    register_builder("max", [] { return std::make_unique<Max>(); });
    register_builder("range", [] { return std::make_unique<Range>(); });
    register_builder("iqr", [] { return std::make_unique<IQR>(); });
    register_builder("q1", [] { return std::make_unique<Quantile>(0.25); });
    register_builder("q3", [] { return std::make_unique<Quantile>(0.75); });
}

}  // namespace stats
