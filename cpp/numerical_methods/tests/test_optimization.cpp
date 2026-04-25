#include "optimization.h"
#include "todo_helper.h"

#include <cmath>

using nm::testing::run_or_todo;
using namespace nm;
using namespace nm::optimization;

int main() {
    // Minimize (x-3)^2 → minimum at x=3.
    run_or_todo("golden_section (x-3)^2", []() {
        double xmin = golden_section([](double x) { return (x - 3) * (x - 3); }, 0.0, 6.0, 1e-8);
        NM_CHECK_NEAR(xmin, 3.0, 1e-6);
    });

    run_or_todo("brent_min (x-3)^2", []() {
        double xmin = brent_min([](double x) { return (x - 3) * (x - 3); }, 0.0, 6.0, 1e-10);
        NM_CHECK_NEAR(xmin, 3.0, 1e-8);
    });

    // Rosenbrock — classic 2-D test.
    run_or_todo("nelder_mead Rosenbrock", []() {
        auto rosen = [](const linalg::Vector& x) {
            double a = 1.0 - x[0];
            double b = x[1] - x[0] * x[0];
            return a * a + 100.0 * b * b;
        };
        auto r = nelder_mead(rosen, {-1.2, 1.0}, 1e-8, 5000);
        NM_CHECK(r.converged);
        NM_CHECK_NEAR(r.x[0], 1.0, 1e-3);
        NM_CHECK_NEAR(r.x[1], 1.0, 1e-3);
    });

    // Linear regression: minimize ||A x - b||^2; with A=[[1,1],[1,2],[1,3]],
    // b=[2,3,4] the solution is x=[1,1].
    run_or_todo("levenberg_marquardt linear fit", []() {
        auto residuals = [](const linalg::Vector& x) {
            return linalg::Vector{
                x[0] + 1.0 * x[1] - 2.0,
                x[0] + 2.0 * x[1] - 3.0,
                x[0] + 3.0 * x[1] - 4.0,
            };
        };
        auto jac = [](const linalg::Vector&) {
            linalg::Matrix J(3, 2);
            J(0, 0) = 1;
            J(0, 1) = 1;
            J(1, 0) = 1;
            J(1, 1) = 2;
            J(2, 0) = 1;
            J(2, 1) = 3;
            return J;
        };
        auto r = levenberg_marquardt(residuals, jac, {0.0, 0.0}, 1e-10, 100);
        NM_CHECK(r.converged);
        NM_CHECK_NEAR(r.x[0], 1.0, 1e-6);
        NM_CHECK_NEAR(r.x[1], 1.0, 1e-6);
    });

    return nm::testing::exit_code();
}
