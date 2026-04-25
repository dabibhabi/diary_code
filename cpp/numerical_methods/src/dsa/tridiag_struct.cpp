#include "c_api/tridiag.h"
#include "dsa/tridiag.h"
#include "todo.h"

#include <stdexcept>

namespace nm::dsa {

// TODO — once nm_thomas_solve is implemented:
//   1. Validate T.valid() && T.size() == rhs.size().
//   2. Make mutable copies of sub/diag/super/rhs.
//   3. Call nm_thomas_solve.
//   4. If rc != 0, throw std::domain_error("zero pivot in tridiagonal solve").
//   5. Return x_out.
std::vector<double> solve(const Tridiag& /*T*/, const std::vector<double>& /*rhs*/) { NM_TODO("dsa::solve(Tridiag)"); }

}  // namespace nm::dsa
