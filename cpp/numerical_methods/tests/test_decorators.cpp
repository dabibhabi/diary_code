// The decorators wrap an arbitrary IMCEstimator. Until you implement a
// concrete one (a GBM-path Monte Carlo engine), this test validates that
// the decorator types instantiate and their estimate() correctly delegates
// to (or throws TODO from) the wrapped base. Real variance-reduction
// assertions land once you have a base estimator + decorators implemented.

#include "patterns/mc_decorators.h"
#include "todo_helper.h"

#include <memory>

using nm::testing::run_or_todo;
using namespace nm::patterns;

namespace {
class DummyEstimator : public IMCEstimator {
public:
    MCResult estimate(int /*n*/, std::uint64_t /*seed*/) const override { return {1.0, 0.1}; }
};
}  // namespace

int main() {
    run_or_todo("AntitheticDecorator wraps an estimator", []() {
        auto                base = std::make_unique<DummyEstimator>();
        AntitheticDecorator d(std::move(base));
        auto                r = d.estimate(100, 42);  // until impl: throws TODO
        NM_CHECK(r.mean == r.mean);                   // any non-NaN return is fine
    });

    run_or_todo("ControlVariateDecorator wraps an estimator", []() {
        auto                    base = std::make_unique<DummyEstimator>();
        ControlVariateDecorator d(std::move(base), [](double) { return 1.0; }, 1.0);
        auto                    r = d.estimate(100, 42);
        NM_CHECK(r.mean == r.mean);
    });

    return nm::testing::exit_code();
}
