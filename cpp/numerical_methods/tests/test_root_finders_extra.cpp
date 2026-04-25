#include "patterns/solvers.h"
#include "todo_helper.h"

#include <cmath>

using nm::testing::run_or_todo;
using namespace nm::patterns;

int main() {
    run_or_todo("HalleySolver sqrt(2)", []() {
        HalleySolver s([](double x) { return x * x - 2.0; }, [](double x) { return 2.0 * x; },
                       [](double) { return 2.0; }, 1.0);
        auto         r = s.solve(1e-12, 200);
        NM_CHECK(r.converged);
        NM_CHECK_NEAR(r.root, std::sqrt(2.0), 1e-10);
    });

    run_or_todo("RiddersSolver sqrt(2)", []() {
        RiddersSolver s([](double x) { return x * x - 2.0; }, 0.0, 2.0);
        auto          r = s.solve(1e-12, 200);
        NM_CHECK(r.converged);
        NM_CHECK_NEAR(r.root, std::sqrt(2.0), 1e-10);
    });

    run_or_todo("MullerSolver log(2)", []() {
        MullerSolver s([](double x) { return std::exp(x) - 2.0; }, 0.0, 0.5, 1.0);
        auto         r = s.solve(1e-12, 200);
        NM_CHECK(r.converged);
        NM_CHECK_NEAR(r.root, std::log(2.0), 1e-10);
    });

    run_or_todo("ChandrupatlaSolver sqrt(2)", []() {
        ChandrupatlaSolver s([](double x) { return x * x - 2.0; }, 0.0, 2.0);
        auto               r = s.solve(1e-12, 200);
        NM_CHECK(r.converged);
        NM_CHECK_NEAR(r.root, std::sqrt(2.0), 1e-10);
    });

    return nm::testing::exit_code();
}
