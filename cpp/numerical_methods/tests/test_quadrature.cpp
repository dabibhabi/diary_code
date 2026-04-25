#include "quadrature.h"
#include "todo_helper.h"

#include <cmath>

using nm::testing::run_or_todo;
using namespace nm::quadrature;

int main() {
    // Reference values:
    //   ∫_0^1 x^2 dx = 1/3
    //   ∫_0^π sin(x) dx = 2
    //   ∫_{-∞}^∞ e^{-x^2} dx = sqrt(π)  (Gauss-Hermite with g(x)=1)
    //   E[Z^2] under N(0,1) = 1            (Gauss-Hermite test)

    run_or_todo("romberg ∫x^2", []() {
        double r = romberg([](double x) { return x * x; }, 0.0, 1.0, 12, 1e-12);
        NM_CHECK_NEAR(r, 1.0 / 3.0, 1e-10);
    });

    run_or_todo("adaptive_simpson ∫sin", []() {
        double r = adaptive_simpson([](double x) { return std::sin(x); }, 0.0, M_PI, 1e-10);
        NM_CHECK_NEAR(r, 2.0, 1e-9);
    });

    run_or_todo("gauss_legendre ∫x^4 [n=4 exact]", []() {
        // 4-point GL is exact for degree <= 7.
        double r = gauss_legendre([](double x) { return x * x * x * x; }, 0.0, 1.0, 4);
        NM_CHECK_NEAR(r, 0.2, 1e-12);
    });

    run_or_todo("gauss_hermite ∫e^{-x^2}", []() {
        // ∫_{-∞}^{∞} e^{-x^2} dx = √π  →  pass g(x) = 1.
        double r = gauss_hermite([](double) { return 1.0; }, 16);
        NM_CHECK_NEAR(r, std::sqrt(M_PI), 1e-8);
    });

    run_or_todo("tanh_sinh ∫1/√(1-x^2)", []() {
        // Endpoint singularity at x = ±1.  ∫_{-1}^1 1/√(1-x^2) dx = π.
        double r = tanh_sinh([](double x) { return 1.0 / std::sqrt(1.0 - x * x); }, -1.0, 1.0, 1e-10, 12);
        NM_CHECK_NEAR(r, M_PI, 1e-6);
    });

    return nm::testing::exit_code();
}
