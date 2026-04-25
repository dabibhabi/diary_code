// Smoke-tests each concrete observer in isolation, without going through a
// solver. Useful when the iterative_root_finder loop isn't yet implemented
// but you've started on the observers.

#include "patterns/convergence_observer.h"
#include "todo_helper.h"

using nm::testing::run_or_todo;
using namespace nm::patterns;

int main() {
    run_or_todo("LoggingObserver runs without throwing", []() {
        LoggingObserver l;
        l.on_iteration({0, 1.0, 0.1, -0.5});  // shouldn't throw
    });

    run_or_todo("CapturingObserver collects events", []() {
        CapturingObserver c;
        c.on_iteration({0, 1.0, 0.5, 0.0});
        c.on_iteration({1, 1.5, 0.25, 0.0});
        NM_CHECK(c.events().size() == 2);
        NM_CHECK(c.events()[1].iter == 1);
    });

    run_or_todo("EarlyStopObserver fires below threshold", []() {
        EarlyStopObserver e(0.1);
        NM_CHECK(!e.should_stop({0, 0.0, 0.5, 0.0}));
        NM_CHECK(e.should_stop({1, 0.0, 0.05, 0.0}));
    });

    return nm::testing::exit_code();
}
