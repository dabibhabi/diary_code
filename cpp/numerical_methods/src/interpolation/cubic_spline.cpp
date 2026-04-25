#include "interpolation.h"
#include "todo.h"

namespace nm::interpolation {

// TODO: evaluate a fitted cubic spline.
//   Binary-search for segment i s.t. x[i] <= xq < x[i+1]; let t = xq - x[i];
//   return a[i] + b[i]*t + c[i]*t*t + d[i]*t*t*t (Horner is faster).
double CubicSpline::operator()(double /*xq*/) const { NM_TODO("CubicSpline::operator()"); }

// TODO: natural cubic spline.
//   Solve a tridiagonal system for the second derivatives M_i with
//   boundary M_0 = M_{n-1} = 0 (the "natural" condition). Then convert
//   (M, x, y) into per-segment {a, b, c, d}. NR3 §3.3 has clean pseudocode.
// LANGUAGE CHOICE: C++; the tridiagonal solve is O(n) and tiny for typical
//   yield-curve sizes (~30 knots).
CubicSpline natural_cubic_spline(const std::vector<double>& /*x*/, const std::vector<double>& /*y*/) {
    NM_TODO("interpolation::natural_cubic_spline");
}

}  // namespace nm::interpolation
