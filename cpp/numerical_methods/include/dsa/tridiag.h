#pragma once

// Strongly-typed tridiagonal-system wrapper.
//
// Why this DSA fits PDE solvers:
//   - The Thomas algorithm (c_api/tridiag.h) takes 4 raw double pointers
//     (a, b, c, d). Easy to pass them in the wrong order.
//   - Wrapping them in a Tridiag struct gives a single type to pass around
//     and a place to attach validation (size consistency) and helpers.

#include <cstddef>
#include <vector>

namespace nm::dsa {

struct Tridiag {
    // Same convention as c_api/tridiag.h:
    //   a[0] is unused (no sub-diagonal entry on row 0)
    //   c[n-1] is unused (no super-diagonal entry on the last row)
    std::vector<double> sub;    // size n; sub[i] is A(i, i-1)
    std::vector<double> diag;   // size n; diag[i] is A(i, i)
    std::vector<double> super;  // size n; super[i] is A(i, i+1)

    std::size_t size() const { return diag.size(); }
    bool        valid() const { return sub.size() == diag.size() && super.size() == diag.size(); }
};

// Solves T * x = rhs in-place on copies (does NOT mutate T or rhs).
// Returns the solution vector or throws std::domain_error on a zero pivot.
// Internally calls c_api::nm_thomas_solve.
std::vector<double> solve(const Tridiag& T, const std::vector<double>& rhs);  // STUB

}  // namespace nm::dsa
