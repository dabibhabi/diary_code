#pragma once

// Observer pattern (behavioral) — decouples iteration tracking from the
// solver loop. A solver fires `on_iteration(...)` after each step; observers
// can log, capture for plotting, or signal early-stop.
//
// Why it fits numerical methods:
//   - Convergence diagnostics (plotting |e_k| vs k) shouldn't require
//     re-running the solver — just attach a CapturingObserver.
//   - Early-stop predicates (e.g. "stop when bracket width < 1e-12") belong
//     outside the algorithm's core logic.

#include <memory>
#include <vector>

namespace nm::patterns {

struct IterationEvent {
    int    iter;       // iteration index (0-based)
    double x;          // current iterate
    double abs_error;  // |x_n - x_{n-1}|, or bracket width, etc.
    double f_value;    // f(x), if cheaply available; else NaN
};

class IConvergenceObserver {
public:
    virtual ~IConvergenceObserver()                  = default;
    virtual void on_iteration(const IterationEvent&) = 0;
    virtual bool should_stop(const IterationEvent&) const { return false; }
};

// Concrete observer #1: log to stderr.
class LoggingObserver : public IConvergenceObserver {
public:
    void on_iteration(const IterationEvent& e) override;  // STUB until you implement
};

// Concrete observer #2: capture every event into a vector for later plotting
// or estimate_order() analysis.
class CapturingObserver : public IConvergenceObserver {
public:
    void                               on_iteration(const IterationEvent& e) override;  // STUB
    const std::vector<IterationEvent>& events() const { return events_; }

private:
    std::vector<IterationEvent> events_;
};

// Concrete observer #3: stop when abs_error drops below a threshold,
// independently of the solver's own tolerance. Useful for testing.
class EarlyStopObserver : public IConvergenceObserver {
public:
    explicit EarlyStopObserver(double threshold) : threshold_(threshold) {}
    void on_iteration(const IterationEvent&) override {}       // no-op
    bool should_stop(const IterationEvent& e) const override;  // STUB

private:
    double threshold_;
};

}  // namespace nm::patterns
