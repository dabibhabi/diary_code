#include "patterns/convergence_observer.h"

#include "todo.h"

#include <cstdio>

namespace nm::patterns {

// TODO — print to stderr in a fixed-width format. Suggested:
//   std::fprintf(stderr, "[iter %3d] x = % .12f  |err| = %.3e  f = %.3e\n",
//                e.iter, e.x, e.abs_error, e.f_value);
void LoggingObserver::on_iteration(const IterationEvent& /*e*/) { NM_TODO("LoggingObserver::on_iteration"); }

// TODO — append e to events_.
void CapturingObserver::on_iteration(const IterationEvent& /*e*/) { NM_TODO("CapturingObserver::on_iteration"); }

// TODO — return e.abs_error < threshold_.
bool EarlyStopObserver::should_stop(const IterationEvent& /*e*/) const { NM_TODO("EarlyStopObserver::should_stop"); }

}  // namespace nm::patterns
