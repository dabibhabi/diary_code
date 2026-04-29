#pragma once

#include <string>

#include "Dataset.hpp"

namespace stats {

struct Interval {
    double lower;
    double upper;
};

// Strategy: produces a confidence interval for some target parameter.
class ConfidenceInterval {
public:
    virtual ~ConfidenceInterval() = default;

    [[nodiscard]] virtual std::string name() const                       = 0;
    [[nodiscard]] virtual Interval    compute(const Dataset& data) const = 0;

protected:
    ConfidenceInterval()                                         = default;
    ConfidenceInterval(const ConfidenceInterval&)                = default;
    ConfidenceInterval(ConfidenceInterval&&) noexcept            = default;
    ConfidenceInterval& operator=(const ConfidenceInterval&)     = default;
    ConfidenceInterval& operator=(ConfidenceInterval&&) noexcept = default;
};

class MeanCIKnownSigma final : public ConfidenceInterval {
public:
    MeanCIKnownSigma(double sigma, double level = 0.95);

    [[nodiscard]] std::string name() const override;
    [[nodiscard]] Interval    compute(const Dataset& data) const override;

private:
    double sigma_;
    double level_;
};

class MeanCIUnknownSigma final : public ConfidenceInterval {
public:
    explicit MeanCIUnknownSigma(double level = 0.95);

    [[nodiscard]] std::string name() const override;
    [[nodiscard]] Interval    compute(const Dataset& data) const override;

private:
    double level_;
};

enum class Alternative { TwoSided, Less, Greater };

struct TestResult {
    double statistic;
    double p_value;
    bool   reject;
};

// Strategy: a hypothesis test that produces a TestResult on a given Dataset.
class HypothesisTest {
public:
    virtual ~HypothesisTest() = default;

    [[nodiscard]] virtual std::string name() const                    = 0;
    [[nodiscard]] virtual TestResult  test(const Dataset& data) const = 0;

protected:
    HypothesisTest()                                     = default;
    HypothesisTest(const HypothesisTest&)                = default;
    HypothesisTest(HypothesisTest&&) noexcept            = default;
    HypothesisTest& operator=(const HypothesisTest&)     = default;
    HypothesisTest& operator=(HypothesisTest&&) noexcept = default;
};

class OneSampleZTest final : public HypothesisTest {
public:
    OneSampleZTest(double mu0, double sigma, Alternative alt = Alternative::TwoSided, double alpha = 0.05);

    [[nodiscard]] std::string name() const override;
    [[nodiscard]] TestResult  test(const Dataset& data) const override;

private:
    double      mu0_;
    double      sigma_;
    Alternative alt_;
    double      alpha_;
};

class OneSampleTTest final : public HypothesisTest {
public:
    OneSampleTTest(double mu0, Alternative alt = Alternative::TwoSided, double alpha = 0.05);

    [[nodiscard]] std::string name() const override;
    [[nodiscard]] TestResult  test(const Dataset& data) const override;

private:
    double      mu0_;
    Alternative alt_;
    double      alpha_;
};

}  // namespace stats
