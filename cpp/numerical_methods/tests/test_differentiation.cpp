#include "differentiation.h"
#include "todo_helper.h"

#include <cmath>
#include <complex>

using nm::testing::run_or_todo;
using namespace nm::differentiation;

int main() {
    run_or_todo("richardson d/dx sin(x) at 1", []() {
        auto df = richardson_derivative([](double x) { return std::sin(x); }, 0.1, 4);
        NM_CHECK_NEAR(df(1.0), std::cos(1.0), 1e-10);
    });

    run_or_todo("complex_step d/dx sin(x) at 1", []() {
        auto   fc = [](std::complex<double> z) { return std::sin(z); };
        double d  = complex_step_derivative(fc, 1.0, 1e-20);
        NM_CHECK_NEAR(d, std::cos(1.0), 1e-12);
    });

    return nm::testing::exit_code();
}
