#include "dsa/fft.h"

#include "dsa/bit_reverse.h"
#include "todo.h"

namespace nm::dsa {

// TODO — reverse(x, bits):
//   uint32_t r = 0;
//   for (int i = 0; i < bits; ++i) { r = (r << 1) | (x & 1); x >>= 1; }
//   return r;
std::uint32_t reverse_bits(std::uint32_t /*x*/, int /*bits*/) { NM_TODO("dsa::reverse_bits"); }

// TODO — Cooley-Tukey radix-2 FFT.
//   1. assert n is a power of 2; let logn = log2(n).
//   2. apply bit-reversal permutation in place.
//   3. for s in 1..logn:
//          m = 1 << s
//          wm = exp(direction * 2*pi*i / m)
//          for k in 0..n step m:
//              w = 1
//              for j in 0..m/2:
//                  t = w * a[k + j + m/2]
//                  u = a[k + j]
//                  a[k + j]       = u + t
//                  a[k + j + m/2] = u - t
//                  w *= wm
//   4. if direction == +1, divide all entries by n.
void fft(std::vector<std::complex<double>>& /*a*/, int /*direction*/) { NM_TODO("dsa::fft"); }

}  // namespace nm::dsa
