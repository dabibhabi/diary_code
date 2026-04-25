#include "random_mc.h"
#include "todo.h"

namespace nm::random_mc {

// TODO: 1-D Sobol = Van der Corput in base 2.
//   For i in 1..n:
//     reverse the bits of i (within k = ceil(log2 n) bits)
//     divide by 2^k → that's the i-th 1-D Sobol point.
//   Multi-dimensional Sobol needs *direction numbers* — Joe & Kuo tables.
// LANGUAGE CHOICE: C/C++ for sequence generation. CUDA's cuRAND has Sobol32
//   for huge batches.
// References: Wikipedia "Sobol_sequence"; NR3 §7.8.
std::vector<double> sobol_1d(int /*n*/) { NM_TODO("random_mc::sobol_1d"); }

}  // namespace nm::random_mc
