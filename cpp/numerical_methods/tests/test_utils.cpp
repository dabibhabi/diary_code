#include "utils.h"

#include <cassert>
#include <cmath>

using namespace nm;

int main() {
    // d/dx sin(x) at 0 = 1
    auto df = utils::derivative([](double x) { return std::sin(x); });
    assert(std::abs(df(0.0) - 1.0) < 1e-6);

    // integral of x^2 on [0,1] = 1/3
    assert(std::abs(utils::integrate([](double x) { return x * x; }, 0.0, 1.0) - 1.0 / 3.0) < 1e-6);

    return 0;
}
