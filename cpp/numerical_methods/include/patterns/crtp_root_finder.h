#pragma once

// §13 STRETCH — CRTP / static polymorphism.
//
// Same shape as IterativeRootFinder, but the dispatch to do_step() is
// resolved at compile time. The compiler can inline the entire iteration
// body, including the function evaluation, into solve(). On a tight loop
// with a cheap f(x), the speedup over virtual dispatch is typically 3-10x.
//
// Why it matters in quant code:
//   - Calibration loops do millions of solves of the same f.
//   - Virtual dispatch + std::function each cost a few ns per call;
//     CRTP eliminates both.
//
// Trade-off: header-only, increases compile time, can't be selected at
// runtime by enum. A library typically offers BOTH: virtual for flexibility,
// CRTP for the hot path.

#include "numerical_methods.h"

#include <cmath>
#include <utility>
#include <vector>

namespace nm::patterns {

template <typename Derived>
class CRTPIterativeRootFinder {
public:
    ConvergenceResult solve(double tol = 1e-10, int max_iter = 200) {
        auto& self = static_cast<Derived&>(*this);
        self.reset_state();
        ConvergenceResult res{0.0, 0, false, {}};
        for (int k = 0; k < max_iter; ++k) {
            double       abs_err = 0.0, fx = 0.0;
            const double x = self.do_step(abs_err, fx);
            res.errors.push_back(abs_err);
            ++res.iterations;
            if (abs_err < tol) {
                res.root      = x;
                res.converged = true;
                return res;
            }
        }
        res.root = self.current_x();
        return res;
    }
};

// Header-only CRTP bisection. Compare against patterns::BisectionSolver
// in the test_crtp_dispatch microbenchmark.
class CRTPBisection : public CRTPIterativeRootFinder<CRTPBisection> {
public:
    CRTPBisection(func f, double a, double b) : f_(std::move(f)), a0_(a), b0_(b) {}

    void reset_state() {
        a_  = a0_;
        b_  = b0_;
        fa_ = f_(a_);
        fb_ = f_(b_);
    }

    double do_step(double& abs_err, double& fx) {
        const double mid = 0.5 * (a_ + b_);
        const double fm  = f_(mid);
        abs_err          = 0.5 * std::abs(b_ - a_);
        fx               = fm;
        if (fa_ * fm < 0) {
            b_  = mid;
            fb_ = fm;
        } else {
            a_  = mid;
            fa_ = fm;
        }
        return mid;
    }

    double current_x() const { return 0.5 * (a_ + b_); }

private:
    func   f_;
    double a0_, b0_, a_{}, b_{}, fa_{}, fb_{};
};

}  // namespace nm::patterns
