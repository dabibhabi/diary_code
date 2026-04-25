#pragma once

// Strategy + Template Method patterns (behavioral).
//
// Strategy: IRootFinder is the abstract interface; each concrete solver
//   (BisectionSolver, NewtonSolver, ...) is a Strategy.
// Template Method: IterativeRootFinder::solve() is non-virtual final and
//   implements the *skeleton* (loop, convergence check, observer fan-out);
//   subclasses override do_step() to implement the *single iteration rule*.
//
// Why it fits numerical methods:
//   - Every iterative root-finder shares 80% of the same loop. Centralizing
//     it eliminates copy-paste bugs (forgetting to record |e_k|, etc.).
//   - The do_step() vtable lookup is amortized over O(1/tol)·log iterations,
//     not the inner work. For tighter loops where dispatch matters, see
//     `crtp_root_finder.h` (§13).

#include "convergence_observer.h"
#include "numerical_methods.h"  // ConvergenceResult, func

#include <memory>
#include <vector>

namespace nm::patterns {

// Strategy interface — every solver returns a ConvergenceResult.
class IRootFinder {
public:
    virtual ~IRootFinder() = default;

    virtual ConvergenceResult solve(double tol = 1e-10, int max_iter = 200) = 0;

    // Observer attachment is non-virtual — observers are managed here.
    void attach(std::shared_ptr<IConvergenceObserver> obs) { observers_.push_back(std::move(obs)); }
    void clear_observers() { observers_.clear(); }

protected:
    // Fan out to all observers; return true if any observer wants to stop.
    bool notify(const IterationEvent& e);

    std::vector<std::shared_ptr<IConvergenceObserver>> observers_;
};

// Template Method base. Subclasses override only do_step() (and optionally
// is_converged() if their convergence criterion isn't simple |Δx| < tol).
class IterativeRootFinder : public IRootFinder {
public:
    // The skeleton. Final — subclasses do NOT override this.
    ConvergenceResult solve(double tol = 1e-10, int max_iter = 200) override;  // STUB

protected:
    // Hooks for subclasses.
    virtual void   reset_state()                                       = 0;
    virtual double do_step(double& abs_error_out, double& f_value_out) = 0;
    virtual bool   is_converged(double abs_error, double tol) const { return abs_error < tol; }
    virtual double current_x() const = 0;
};

}  // namespace nm::patterns
