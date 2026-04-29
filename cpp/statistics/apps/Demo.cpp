#include "Demo.hpp"

#include <iomanip>
#include <iostream>
#include <string>

#include "Inference.hpp"
#include "MLE.hpp"
#include "OrderStatistic.hpp"
#include "Pipeline.hpp"
#include "StatisticFactory.hpp"

namespace stats::demo {

namespace {

void section(const std::string& title) { std::cout << '\n' << "=== " << title << " ===" << '\n'; }

void print_results(const std::vector<NamedResult>& results) {
    for (const auto& r : results) {
        std::cout << "  " << std::setw(22) << std::left << r.name << " = " << r.value << '\n';
    }
}

void print_estimate(const Estimator& est, const Dataset& data) {
    std::cout << "  " << std::setw(22) << std::left << est.name() << " : ";
    bool first = true;
    for (const auto& e : est.estimate(data)) {
        if (!first) std::cout << ", ";
        std::cout << e.parameter << " = " << e.value;
        first = false;
    }
    std::cout << '\n';
}

void print_ci(const ConfidenceInterval& ci, const Dataset& data) {
    const auto interval = ci.compute(data);
    std::cout << "  " << std::setw(32) << std::left << ci.name() << " = (" << interval.lower << ", " << interval.upper
              << ")\n";
}

void print_test(const HypothesisTest& t, const Dataset& data) {
    const auto r = t.test(data);
    std::cout << "  " << std::setw(22) << std::left << t.name() << " : stat = " << r.statistic << ", p = " << r.p_value
              << ", reject = " << std::boolalpha << r.reject << '\n';
}

}  // namespace

void run_descriptive_stats(const Dataset& data) {
    section("Descriptive Statistics");
    auto& factory = StatisticFactory::instance();
    factory.register_builtins();

    StatisticsPipeline desc;
    for (const auto& key : {"min", "max", "mean", "median", "var", "stddev"}) {
        desc.add(factory.create(key));
    }
    print_results(desc.run(data));
}

void run_order_stats(const Dataset& data) {
    section("Order Statistics");
    StatisticsPipeline order;
    order.emplace<OrderStatistic>(3)
        .emplace<Quantile>(0.25)
        .emplace<Quantile>(0.50)
        .emplace<Quantile>(0.75)
        .emplace<Range>()
        .emplace<IQR>();
    print_results(order.run(data));
}

void run_mle() {
    section("Point Estimation (MLE)");
    const Dataset normal_sample{2.0, 4.0, 4.0, 4.0, 5.0, 5.0, 7.0, 9.0};
    const Dataset exp_sample{0.5, 1.2, 0.8, 2.1, 1.7, 0.3, 1.0};
    const Dataset bern_sample{1.0, 0.0, 1.0, 1.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0};
    const Dataset poisson_sample{3.0, 1.0, 4.0, 1.0, 5.0, 9.0, 2.0, 6.0};

    print_estimate(NormalMLE{}, normal_sample);
    print_estimate(ExponentialMLE{}, exp_sample);
    print_estimate(BernoulliMLE{}, bern_sample);
    print_estimate(PoissonMLE{}, poisson_sample);
}

void run_inference(const Dataset& data) {
    section("Inference");
    print_ci(MeanCIKnownSigma{2.0, 0.95}, data);
    print_ci(MeanCIUnknownSigma{0.95}, data);
    print_test(OneSampleZTest{5.0, 2.0}, data);
    print_test(OneSampleTTest{5.0}, data);
    print_test(OneSampleTTest{7.0}, data);
}

void run_all() {
    std::cout << std::fixed << std::setprecision(4);

    const Dataset data{2.0, 4.0, 4.0, 4.0, 5.0, 5.0, 7.0, 9.0};
    std::cout << "n = " << data.size() << '\n';

    run_descriptive_stats(data);
    run_order_stats(data);
    run_mle();
    run_inference(data);
}

}  // namespace stats::demo
