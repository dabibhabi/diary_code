#pragma once

// Min-heap (priority queue) keyed by absolute error, used by adaptive
// quadrature to process the highest-error subinterval first.
//
// Why this DSA fits adaptive Simpson:
//   - Naive recursive subdivision touches every subinterval blindly.
//   - A heap lets you `pop_max_error()` in O(log n) and stop the moment
//     the *current max* drops below tolerance — a much tighter halt
//     condition than "all leaves passed".
//   - This is how production adaptive quadrature (QUADPACK, scipy.integrate)
//     is structured.

#include <cstddef>
#include <vector>

namespace nm::dsa {

struct Subinterval {
    double a;
    double b;
    double estimate;   // Simpson estimate on [a, b]
    double error_est;  // |Simpson(a,b) - (Simpson(a,m) + Simpson(m,b))| / 15
};

// Max-heap on `error_est` (we want largest error at top). Templated only
// over the comparator-policy if you want, but the concrete type is fine
// here — this is a 1-purpose DS for one algorithm.
class ErrorHeap {
public:
    void               push(const Subinterval& s);  // STUB
    Subinterval        pop_max();                   // STUB — undefined if empty
    const Subinterval& peek_max() const;            // STUB
    std::size_t        size() const { return data_.size(); }
    bool               empty() const { return data_.empty(); }

private:
    std::vector<Subinterval> data_;
    void                     sift_up(std::size_t i);    // STUB
    void                     sift_down(std::size_t i);  // STUB
};

}  // namespace nm::dsa
