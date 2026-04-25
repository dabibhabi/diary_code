#pragma once

#include <vector>

// 1-D interpolation. See TODO.md §8.
namespace nm::interpolation {

// Stores knots (x) and per-segment cubic coefficients.
struct CubicSpline {
    std::vector<double> x;               // sorted knots
    std::vector<double> a, b, c, d;      // S_i(t) = a_i + b_i t + c_i t^2 + d_i t^3
                                         // with t = x - x_i, evaluated on [x_i, x_{i+1}]
    double operator()(double xq) const;  // evaluation (binary search + Horner)
};

// Natural cubic spline — second derivative = 0 at both endpoints.
// References: Wikipedia "Spline_interpolation"; NR3 §3.3.
CubicSpline natural_cubic_spline(const std::vector<double>& x, const std::vector<double>& y);

// Monotone cubic interpolation (Hyman / Steffen filter). Modifies the
// spline slopes so the result is monotone wherever the data is monotone —
// QUANT: avoids negative forwards in yield-curve interpolation.
// References: Hagan & West, "Interpolation Methods for Curve Construction".
CubicSpline monotone_cubic(const std::vector<double>& x, const std::vector<double>& y);

// PCHIP — Fritsch-Carlson piecewise cubic Hermite. The same flavor as
// MATLAB/SciPy default. References: Fritsch & Carlson 1980.
CubicSpline pchip(const std::vector<double>& x, const std::vector<double>& y);

}  // namespace nm::interpolation
