#include "random_mc.h"
#include "todo_helper.h"

#include <cmath>
#include <vector>

using nm::testing::run_or_todo;
using namespace nm::random_mc;

int main() {
    run_or_todo("box_muller produces unit-variance pair", []() {
        // Spot-check: with u1=u2=0.5,
        //   r = sqrt(-2 ln 0.5) = sqrt(2 ln 2) ≈ 1.1774
        //   z1 = r*cos(π) = -r,  z2 = r*sin(π) = 0  (numerically tiny)
        auto   p = box_muller(0.5, 0.5);
        double r = std::sqrt(2.0 * std::log(2.0));
        NM_CHECK_NEAR(p.z1, -r, 1e-10);
        NM_CHECK_NEAR(p.z2, 0.0, 1e-10);
    });

    run_or_todo("inv_normal_cdf central + tails", []() {
        NM_CHECK_NEAR(inv_normal_cdf(0.5), 0.0, 1e-10);
        NM_CHECK_NEAR(inv_normal_cdf(0.8413447), 1.0, 1e-5);
        NM_CHECK_NEAR(inv_normal_cdf(0.9999), 3.71902, 1e-3);
    });

    run_or_todo("sobol_1d first 8 points", []() {
        // 1-D Sobol = bit-reversal/2^k. The first 8 points are
        // 0.5, 0.25, 0.75, 0.125, 0.625, 0.375, 0.875, 0.0625.
        auto                p        = sobol_1d(8);
        std::vector<double> expected = {0.5, 0.25, 0.75, 0.125, 0.625, 0.375, 0.875, 0.0625};
        for (size_t i = 0; i < expected.size(); ++i) { NM_CHECK_NEAR(p[i], expected[i], 1e-12); }
    });

    return nm::testing::exit_code();
}
