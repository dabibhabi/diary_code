#include "utils.h"

namespace nm::utils {

// Central-difference derivative. O(h^2) accuracy — good enough for root-finding.
func derivative(func f, double h) {
    return [f, h](double x) { return (f(x + h) - f(x - h)) / (2.0 * h); };
}

// Composite Simpson's rule. n must be even; we round up if not.
double integrate(func f, double a, double b, int n) {
    if (n % 2 != 0) ++n;
    const double h = (b - a) / n;
    double       s = f(a) + f(b);
    for (int i = 1; i < n; ++i) {
        const double x = a + i * h;
        s += (i % 2 == 0 ? 2.0 : 4.0) * f(x);
    }
    return s * h / 3.0;
}

}  // namespace nm::utils
