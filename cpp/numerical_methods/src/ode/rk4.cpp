#include "ode.h"
#include "todo.h"

namespace nm::ode {

// TODO: Classical Runge-Kutta 4 stages.
//   k1 = f(t, y)
//   k2 = f(t + h/2, y + h/2 * k1)
//   k3 = f(t + h/2, y + h/2 * k2)
//   k4 = f(t + h,   y + h   * k3)
//   y_{n+1} = y_n + h/6 * (k1 + 2 k2 + 2 k3 + k4)
// LANGUAGE CHOICE: C++ for the scalar/single-trajectory case. Embarrassingly
// parallel across many trajectories → CUDA.
// References: Wikipedia "Runge-Kutta_methods"; NR3 §17.1.
Trajectory rk4(RHS /*f*/, double /*t0*/, State /*y0*/, double /*t_end*/, double /*h*/) { NM_TODO("ode::rk4"); }

}  // namespace nm::ode
