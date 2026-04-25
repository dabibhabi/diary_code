#include "random_mc.h"
#include "todo.h"

namespace nm::random_mc {

// TODO: Box-Muller.
//   r = sqrt(-2 * ln(u1)); θ = 2π u2;
//   z1 = r * cos(θ);  z2 = r * sin(θ).
// LANGUAGE CHOICE: C/C++ for paths-on-CPU; CUDA for batch path generation
//   (use cuRAND's normal kernel — already implements this). Don't reinvent
//   cuRAND for production paths.
NormalPair box_muller(double /*u1*/, double /*u2*/) { NM_TODO("random_mc::box_muller"); }

}  // namespace nm::random_mc
