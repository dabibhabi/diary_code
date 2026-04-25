#include "interpolation.h"
#include "todo.h"

namespace nm::interpolation {

// TODO: PCHIP — Fritsch-Carlson piecewise cubic Hermite.
//   Slopes:
//     m_i = weighted harmonic mean of δ_{i-1} and δ_i if same sign, else 0.
//   Weighted form: m_i = (w1 + w2) / (w1/δ_{i-1} + w2/δ_i),
//     w1 = 2 h_i + h_{i-1},  w2 = h_i + 2 h_{i-1}.
// References: Fritsch & Carlson 1980.
CubicSpline pchip(const std::vector<double>& /*x*/, const std::vector<double>& /*y*/) {
    NM_TODO("interpolation::pchip");
}

}  // namespace nm::interpolation
