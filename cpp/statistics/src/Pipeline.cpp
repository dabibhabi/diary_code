#include "Pipeline.hpp"

#include <stdexcept>
#include <utility>

namespace stats {

StatisticsPipeline& StatisticsPipeline::add(std::unique_ptr<Statistic> stat) {
    if (!stat) throw std::invalid_argument("StatisticsPipeline::add: null Statistic");
    stats_.push_back(std::move(stat));
    return *this;
}

std::vector<NamedResult> StatisticsPipeline::run(const Dataset& data) const {
    std::vector<NamedResult> out;
    out.reserve(stats_.size());
    for (const auto& s : stats_)
        out.push_back({s->name(), s->compute(data)});
    return out;
}

std::size_t StatisticsPipeline::size() const noexcept { return stats_.size(); }

}  // namespace stats
