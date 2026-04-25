#include "patterns/solvers.h"

#include <algorithm>
#include <cmath>
#include <utility>

namespace nm::patterns {

BrentSolver::BrentSolver(func f, double a, double b) : f_(std::move(f)), a0_(a), b0_(b) {}

void BrentSolver::reset_state() {
    a_  = a0_;
    b_  = b0_;
    fa_ = f_(a_);
    fb_ = f_(b_);
    if (std::abs(fa_) < std::abs(fb_)) {
        std::swap(a_, b_);
        std::swap(fa_, fb_);
    }
    c_     = a_;
    fc_    = fa_;
    d_     = c_;
    mflag_ = true;
}

// One step of Brent-Dekker (inverse-quadratic interpolation, secant fallback,
// bisection safeguard). See docs/PATTERNS.md or Brent (1973) for the rule.
double BrentSolver::do_step(double& abs_error_out, double& f_value_out) {
    if (fb_ == 0.0) {
        abs_error_out = 0.0;
        f_value_out   = 0.0;
        return b_;
    }

    double s;
    if (fa_ != fc_ && fb_ != fc_) {
        s = a_ * fb_ * fc_ / ((fa_ - fb_) * (fa_ - fc_)) + b_ * fa_ * fc_ / ((fb_ - fa_) * (fb_ - fc_)) +
            c_ * fa_ * fb_ / ((fc_ - fa_) * (fc_ - fb_));
    } else {
        s = b_ - fb_ * (b_ - a_) / (fb_ - fa_);
    }

    const double lo     = std::min((3.0 * a_ + b_) / 4.0, b_);
    const double hi     = std::max((3.0 * a_ + b_) / 4.0, b_);
    const bool   reject = (s < lo || s > hi) || (mflag_ && std::abs(s - b_) >= 0.5 * std::abs(b_ - c_)) ||
                          (!mflag_ && std::abs(s - b_) >= 0.5 * std::abs(c_ - d_));
    if (reject) {
        s      = 0.5 * (a_ + b_);
        mflag_ = true;
    } else {
        mflag_ = false;
    }

    const double fs = f_(s);
    d_              = c_;
    c_              = b_;
    fc_             = fb_;
    if (fa_ * fs < 0) {
        b_  = s;
        fb_ = fs;
    } else {
        a_  = s;
        fa_ = fs;
    }
    if (std::abs(fa_) < std::abs(fb_)) {
        std::swap(a_, b_);
        std::swap(fa_, fb_);
    }

    abs_error_out = std::abs(b_ - a_);
    f_value_out   = fb_;
    return b_;
}

}  // namespace nm::patterns
