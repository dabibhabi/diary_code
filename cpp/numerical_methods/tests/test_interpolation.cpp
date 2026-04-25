#include "interpolation.h"
#include "todo_helper.h"

#include <cmath>
#include <vector>

using nm::testing::run_or_todo;
using namespace nm::interpolation;

int main() {
    // Sample y = sin(x) at 9 knots over [0, π].
    std::vector<double> xs, ys;
    for (int i = 0; i < 9; ++i) {
        double x = i * M_PI / 8.0;
        xs.push_back(x);
        ys.push_back(std::sin(x));
    }

    run_or_todo("natural_cubic_spline interpolates exactly at knots", [&]() {
        auto sp = natural_cubic_spline(xs, ys);
        for (size_t i = 0; i < xs.size(); ++i) NM_CHECK_NEAR(sp(xs[i]), ys[i], 1e-12);
        // Off-knot accuracy
        NM_CHECK_NEAR(sp(M_PI / 2.0), 1.0, 1e-3);
    });

    run_or_todo("monotone_cubic preserves monotonicity", [&]() {
        std::vector<double> mx   = {0, 1, 2, 3, 4};
        std::vector<double> my   = {0, 1, 1, 2, 4};  // monotone non-decreasing
        auto                sp   = monotone_cubic(mx, my);
        double              prev = sp(0.0);
        for (double xq = 0.0; xq <= 4.0; xq += 0.05) {
            double v = sp(xq);
            NM_CHECK(v >= prev - 1e-12);
            prev = v;
        }
    });

    run_or_todo("pchip exact at knots", [&]() {
        auto sp = pchip(xs, ys);
        for (size_t i = 0; i < xs.size(); ++i) NM_CHECK_NEAR(sp(xs[i]), ys[i], 1e-12);
    });

    return nm::testing::exit_code();
}
