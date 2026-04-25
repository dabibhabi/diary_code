#include "patterns/solvers.h"
#include "todo_helper.h"

#include <cmath>

using nm::testing::run_or_todo;
using namespace nm::patterns;

int main() {
    run_or_todo("BisectionSolver sqrt(2)", []() {
        BisectionSolver s([](double x) { return x * x - 2.0; }, 0.0, 2.0);
        auto            r = s.solve(1e-10, 200);
        NM_CHECK(r.converged);
        NM_CHECK_NEAR(r.root, std::sqrt(2.0), 1e-9);
    });

    run_or_todo("BisectionSolver fails on no sign change", []() {
        BisectionSolver s([](double x) { return x * x + 1.0; }, -1.0, 1.0);
        auto            r = s.solve(1e-10, 200);
        NM_CHECK(!r.converged);
    });

    return nm::testing::exit_code();
}
