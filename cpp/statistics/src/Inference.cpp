#include "Inference.hpp"

#include <cmath>
#include <stdexcept>

#include "MathUtils.hpp"
#include "Mean.hpp"
#include "StandardDeviation.hpp"

namespace stats {

namespace {

void require_level(double level, const char* who) {
    if (level <= 0.0 || level >= 1.0) throw std::invalid_argument(std::string{who} + ": level must be in (0, 1)");
}

void require_alpha(double alpha, const char* who) {
    if (alpha <= 0.0 || alpha >= 1.0) throw std::invalid_argument(std::string{who} + ": alpha must be in (0, 1)");
}

double p_value_from_z(double z, Alternative alt) {
    using stats::math::normal_cdf;
    switch (alt) {
        case Alternative::TwoSided:
            return 2.0 * (1.0 - normal_cdf(std::fabs(z)));
        case Alternative::Less:
            return normal_cdf(z);
        case Alternative::Greater:
            return 1.0 - normal_cdf(z);
    }
    return 1.0;
}

double p_value_from_t(double t, double df, Alternative alt) {
    using stats::math::t_cdf;
    switch (alt) {
        case Alternative::TwoSided:
            return 2.0 * (1.0 - t_cdf(std::fabs(t), df));
        case Alternative::Less:
            return t_cdf(t, df);
        case Alternative::Greater:
            return 1.0 - t_cdf(t, df);
    }
    return 1.0;
}

}  // namespace

MeanCIKnownSigma::MeanCIKnownSigma(double sigma, double level) : sigma_(sigma), level_(level) {
    if (sigma_ <= 0.0) throw std::invalid_argument("MeanCIKnownSigma: sigma must be positive");
    require_level(level_, "MeanCIKnownSigma");
}

std::string MeanCIKnownSigma::name() const { return "CI(mean | sigma known)"; }

Interval MeanCIKnownSigma::compute(const Dataset& data) const {
    if (data.empty()) throw std::invalid_argument("MeanCIKnownSigma: dataset is empty");
    const double xbar = Mean{}.compute(data);
    const double z    = stats::math::normal_quantile(0.5 + level_ / 2.0);
    const double half = z * sigma_ / std::sqrt(static_cast<double>(data.size()));
    return {xbar - half, xbar + half};
}

MeanCIUnknownSigma::MeanCIUnknownSigma(double level) : level_(level) { require_level(level_, "MeanCIUnknownSigma"); }

std::string MeanCIUnknownSigma::name() const { return "CI(mean | sigma unknown)"; }

Interval MeanCIUnknownSigma::compute(const Dataset& data) const {
    if (data.size() < 2) throw std::invalid_argument("MeanCIUnknownSigma: need at least 2 values");
    const double xbar = Mean{}.compute(data);
    const double s    = StandardDeviation{true}.compute(data);
    const double df   = static_cast<double>(data.size()) - 1.0;
    const double t    = stats::math::t_quantile(0.5 + level_ / 2.0, df);
    const double half = t * s / std::sqrt(static_cast<double>(data.size()));
    return {xbar - half, xbar + half};
}

OneSampleZTest::OneSampleZTest(double mu0, double sigma, Alternative alt, double alpha)
    : mu0_(mu0), sigma_(sigma), alt_(alt), alpha_(alpha) {
    if (sigma_ <= 0.0) throw std::invalid_argument("OneSampleZTest: sigma must be positive");
    require_alpha(alpha_, "OneSampleZTest");
}

std::string OneSampleZTest::name() const { return "one-sample z-test"; }

TestResult OneSampleZTest::test(const Dataset& data) const {
    if (data.empty()) throw std::invalid_argument("OneSampleZTest: dataset is empty");
    const double xbar = Mean{}.compute(data);
    const double z    = (xbar - mu0_) / (sigma_ / std::sqrt(static_cast<double>(data.size())));
    const double p    = p_value_from_z(z, alt_);
    return {z, p, p < alpha_};
}

OneSampleTTest::OneSampleTTest(double mu0, Alternative alt, double alpha) : mu0_(mu0), alt_(alt), alpha_(alpha) {
    require_alpha(alpha_, "OneSampleTTest");
}

std::string OneSampleTTest::name() const { return "one-sample t-test"; }

TestResult OneSampleTTest::test(const Dataset& data) const {
    if (data.size() < 2) throw std::invalid_argument("OneSampleTTest: need at least 2 values");
    const double xbar = Mean{}.compute(data);
    const double s    = StandardDeviation{true}.compute(data);
    const double df   = static_cast<double>(data.size()) - 1.0;
    const double t    = (xbar - mu0_) / (s / std::sqrt(static_cast<double>(data.size())));
    const double p    = p_value_from_t(t, df, alt_);
    return {t, p, p < alpha_};
}

}  // namespace stats
