#pragma once

// Cooley-Tukey radix-2 FFT (stretch §13/§14).
//
// Why FFT belongs here for a quant learner:
//   - Carr-Madan (1999) FFT-based option pricing: prices European calls
//     under any model with a known characteristic function (Heston, Levy)
//     in O(N log N) where N is the FFT size.
//   - The DSA itself is a beautiful divide-and-conquer + bit-reversal
//     application — and the radix-2 inner butterfly is one of the most
//     bandwidth-bound kernels you'll ever look at.

#include <complex>
#include <vector>

namespace nm::dsa {

// In-place radix-2 FFT. n = a.size() must be a power of 2.
// Direction: -1 for forward, +1 for inverse. (Inverse normalizes by 1/n.)
// TODO: implement using bit-reversal permutation + log2(n) butterfly passes.
void fft(std::vector<std::complex<double>>& a, int direction);

}  // namespace nm::dsa
