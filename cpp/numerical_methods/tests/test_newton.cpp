#include "numerical_methods.h"
#include "patterns/solvers.h"
#include "todo_helper.h"

#include <cmath>

using nm::testing::run_or_todo;
using namespace nm::patterns;

int main() {
    run_or_todo("NewtonSolver sqrt(2)", []() {
        NewtonSolver s([](double x) { return x * x - 2.0; }, [](double x) { return 2.0 * x; }, 1.0);
        auto         r = s.solve(1e-12, 200);
        NM_CHECK(r.converged);
        NM_CHECK_NEAR(r.root, std::sqrt(2.0), 1e-10);
        NM_CHECK(nm::estimate_order(r.errors) > 1.7);
    });

    return nm::testing::exit_code();
}
