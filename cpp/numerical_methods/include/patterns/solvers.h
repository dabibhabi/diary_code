#pragma once

// Concrete IterativeRootFinder subclasses.
//
// The four classical methods (Bisection/Newton/Secant/Brent) are FULLY
// IMPLEMENTED — their algorithm bodies are ported from the previous static-
// method versions. The four extras (Halley/Ridders/Muller/Chandrupatla) are
// STUBS that throw NM_TODO from do_step().
//
// To make any of them runnable end-to-end, you must first implement
// IterativeRootFinder::solve() in patterns/iterative_root_finder.cpp.

#include "iterative_root_finder.h"

namespace nm::patterns {

class BisectionSolver final : public IterativeRootFinder {
public:
    BisectionSolver(func f, double a, double b);

protected:
    void   reset_state() override;
    double do_step(double& abs_error_out, double& f_value_out) override;
    double current_x() const override { return mid_; }

private:
    func   f_;
    double a0_, b0_;
    double a_{}, b_{}, mid_{};
    double fa_{}, fb_{};
};

class NewtonSolver final : public IterativeRootFinder {
public:
    NewtonSolver(func f, func df, double x0);

protected:
    void   reset_state() override;
    double do_step(double& abs_error_out, double& f_value_out) override;
    double current_x() const override { return x_; }

private:
    func   f_;
    func   df_;
    double x0_, x_{};
};

class SecantSolver final : public IterativeRootFinder {
public:
    SecantSolver(func f, double x0, double x1);

protected:
    void   reset_state() override;
    double do_step(double& abs_error_out, double& f_value_out) override;
    double current_x() const override { return x1_; }

private:
    func   f_;
    double x0_init_, x1_init_;
    double x0_{}, x1_{}, f0_{}, f1_{};
};

class BrentSolver final : public IterativeRootFinder {
public:
    BrentSolver(func f, double a, double b);

protected:
    void   reset_state() override;
    double do_step(double& abs_error_out, double& f_value_out) override;
    double current_x() const override { return b_; }

private:
    func   f_;
    double a0_, b0_;
    double a_{}, b_{}, c_{}, d_{};
    double fa_{}, fb_{}, fc_{};
    bool   mflag_{true};
};

// ---- STUBS (for you to implement) ----

class HalleySolver final : public IterativeRootFinder {
public:
    HalleySolver(func f, func df, func d2f, double x0);

protected:
    void   reset_state() override;
    double do_step(double& abs_error_out, double& f_value_out) override;
    double current_x() const override { return x_; }

private:
    func   f_, df_, d2f_;
    double x0_, x_{};
};

class RiddersSolver final : public IterativeRootFinder {
public:
    RiddersSolver(func f, double a, double b);

protected:
    void   reset_state() override;
    double do_step(double& abs_error_out, double& f_value_out) override;
    double current_x() const override { return x_; }

private:
    func   f_;
    double a0_, b0_;
    double a_{}, b_{}, x_{};
};

class MullerSolver final : public IterativeRootFinder {
public:
    MullerSolver(func f, double x0, double x1, double x2);

protected:
    void   reset_state() override;
    double do_step(double& abs_error_out, double& f_value_out) override;
    double current_x() const override { return x2_; }

private:
    func   f_;
    double x0_init_, x1_init_, x2_init_;
    double x0_{}, x1_{}, x2_{};
};

class ChandrupatlaSolver final : public IterativeRootFinder {
public:
    ChandrupatlaSolver(func f, double a, double b);

protected:
    void   reset_state() override;
    double do_step(double& abs_error_out, double& f_value_out) override;
    double current_x() const override { return b_; }

private:
    func   f_;
    double a0_, b0_;
    double a_{}, b_{};
};

}  // namespace nm::patterns
