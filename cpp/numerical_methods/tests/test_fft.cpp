#include "dsa/bit_reverse.h"
#include "dsa/fft.h"
#include "todo_helper.h"

#include <cmath>
#include <complex>

using nm::testing::run_or_todo;
using namespace nm::dsa;

int main() {
    run_or_todo("bit-reversal of 0..7 in 3 bits", []() {
        // 0->0, 1->4, 2->2, 3->6, 4->1, 5->5, 6->3, 7->7
        NM_CHECK(reverse_bits(1, 3) == 4);
        NM_CHECK(reverse_bits(3, 3) == 6);
        NM_CHECK(reverse_bits(7, 3) == 7);
    });

    run_or_todo("forward+inverse FFT recovers input", []() {
        std::vector<std::complex<double>> a    = {1, 2, 3, 4, 5, 6, 7, 8};
        auto                              orig = a;
        fft(a, -1);  // forward
        fft(a, +1);  // inverse (with 1/n)
        for (size_t i = 0; i < orig.size(); ++i) { NM_CHECK(std::abs(a[i] - orig[i]) < 1e-9); }
    });

    return nm::testing::exit_code();
}
