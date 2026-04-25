#include "patterns/solvers.h"
#include "todo_helper.h"

#include <cmath>

using nm::testing::run_or_todo;
using namespace nm::patterns;

int main() {
    run_or_todo("SecantSolver log(2)", []() {
        SecantSolver s([](double x) { return std::exp(x) - 2.0; }, 0.0, 1.0);
        auto         r = s.solve(1e-12, 200);
        NM_CHECK(r.converged);
        NM_CHECK_NEAR(r.root, std::log(2.0), 1e-10);
    });

    return nm::testing::exit_code();
}
