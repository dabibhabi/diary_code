#pragma once

// Bit-reversal permutation — prerequisite for in-place radix-2 FFT.
//
// Why this DSA fits FFT:
//   - Cooley-Tukey divides the input by even/odd indices; after log2(N)
//     levels, sample i lives at position bitreverse(i, log2 N).
//   - A precomputed bit-reversal table or in-place swap pass turns this
//     into an O(N) reorder; without it, you're either copying or doing
//     log-time index math per access.

#include <cstdint>
#include <vector>

namespace nm::dsa {

// Reverse the lowest `bits` bits of x. (E.g. reverse_bits(1, 3) == 0b100 == 4.)
// TODO: implement.
std::uint32_t reverse_bits(std::uint32_t x, int bits);

// Apply the bit-reversal permutation in place. n must be a power of 2.
// TODO: implement using reverse_bits and an in-place swap.
template <typename T>
void apply_bit_reverse(std::vector<T>& a);  // declaration only; user provides impl

}  // namespace nm::dsa
