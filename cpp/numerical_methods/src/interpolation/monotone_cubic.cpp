#include "interpolation.h"
#include "todo.h"

namespace nm::interpolation {

// TODO: monotone cubic interpolation (Hyman / Steffen filter).
//   Build the "raw" Hermite slopes m_i, then apply the filter:
//     if sign(δ_{i-1}) != sign(δ_i)  →  m_i = 0      (extremum: flat tangent)
//     else clamp m_i to ≤ 3*min(|δ_{i-1}|, |δ_i|)    (Hyman / Fritsch–Carlson)
//   where δ_i = (y_{i+1} - y_i)/(x_{i+1} - x_i).
// QUANT USE: yield-curve interpolation — Hagan & West warn that plain cubic
//   spline can produce negative forward rates; monotone cubic prevents it.
CubicSpline monotone_cubic(const std::vector<double>& /*x*/, const std::vector<double>& /*y*/) {
    NM_TODO("interpolation::monotone_cubic");
}

}  // namespace nm::interpolation
