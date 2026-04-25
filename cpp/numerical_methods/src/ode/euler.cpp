#include "ode.h"
#include "todo.h"

namespace nm::ode {

// TODO: Forward Euler.
//   y_{n+1} = y_n + h * f(t_n, y_n)
// LANGUAGE CHOICE: C++ for clarity; if you need to integrate millions of
// independent SDE paths in parallel, port the inner loop to CUDA — each
// thread owns one path (see cuda/monte_carlo_paths.cu).
Trajectory euler(RHS /*f*/, double /*t0*/, State /*y0*/, double /*t_end*/, double /*h*/) { NM_TODO("ode::euler"); }

}  // namespace nm::ode
