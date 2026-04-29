#include "MLE.hpp"

#include <numeric>
#include <stdexcept>

namespace stats {

namespace {

double mean_of(const Dataset& data) {
    const auto& v = data.values();
    return std::accumulate(v.begin(), v.end(), 0.0) / static_cast<double>(v.size());
}

void require_nonempty(const Dataset& data, const char* who) {
    if (data.empty()) throw std::invalid_argument(std::string{who} + ": dataset is empty");
}

}  // namespace

std::string NormalMLE::name() const { return "Normal MLE"; }

std::vector<Estimate> NormalMLE::estimate(const Dataset& data) const {
    require_nonempty(data, "NormalMLE");
    const double mu = mean_of(data);
    double       ss = 0.0;
    for (double x : data.values()) {
        const double d = x - mu;
        ss += d * d;
    }
    const double sigma2 = ss / static_cast<double>(data.size());
    return {{"mu", mu}, {"sigma^2", sigma2}};
}

std::string ExponentialMLE::name() const { return "Exponential MLE"; }

std::vector<Estimate> ExponentialMLE::estimate(const Dataset& data) const {
    require_nonempty(data, "ExponentialMLE");
    for (double x : data.values())
        if (x <= 0.0) throw std::invalid_argument("ExponentialMLE: values must be positive");
    const double xbar = mean_of(data);
    return {{"lambda", 1.0 / xbar}};
}

std::string BernoulliMLE::name() const { return "Bernoulli MLE"; }

std::vector<Estimate> BernoulliMLE::estimate(const Dataset& data) const {
    require_nonempty(data, "BernoulliMLE");
    for (double x : data.values())
        if (x != 0.0 && x != 1.0) throw std::invalid_argument("BernoulliMLE: values must be 0 or 1");
    return {{"p", mean_of(data)}};
}

std::string PoissonMLE::name() const { return "Poisson MLE"; }

std::vector<Estimate> PoissonMLE::estimate(const Dataset& data) const {
    require_nonempty(data, "PoissonMLE");
    for (double x : data.values())
        if (x < 0.0) throw std::invalid_argument("PoissonMLE: values must be non-negative");
    return {{"lambda", mean_of(data)}};
}

}  // namespace stats
