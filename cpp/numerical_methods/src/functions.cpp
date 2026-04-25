#include "functions.h"

#include <cmath>

namespace nm {

// Dispatch table of test functions. Each entry includes a bracket [a,b]
// for bracketing methods and seeds x0,x1 for open methods.
const std::vector<TestFunction>& test_functions() {
    static const std::vector<TestFunction> fns = {
        {
            "x^2 - 2",
            [](double x) { return x * x - 2.0; },
            0.0,
            2.0,
            1.0,
            2.0,
        },
        {
            "sin(x)  [root near pi]",
            [](double x) { return std::sin(x); },
            3.0,
            4.0,
            3.0,
            4.0,
        },
        {
            "exp(x) - 2",
            [](double x) { return std::exp(x) - 2.0; },
            0.0,
            1.0,
            0.0,
            1.0,
        },
        {
            "x^3 - x - 2",
            [](double x) { return x * x * x - x - 2.0; },
            1.0,
            2.0,
            1.5,
            2.0,
        },
    };
    return fns;
}

}  // namespace nm
