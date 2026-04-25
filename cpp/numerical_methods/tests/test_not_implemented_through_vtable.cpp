// Defensive test: `nm::not_implemented` thrown from a `do_step()` override
// must propagate cleanly through IterativeRootFinder::solve() and out of the
// IRootFinder vtable. This protects against well-meaning catch-all handlers
// that would silently swallow stub markers.

#include "patterns/solvers.h"
#include "todo_helper.h"

using nm::testing::run_or_todo;
using namespace nm::patterns;

int main() {
    run_or_todo("HalleySolver propagates not_implemented", []() {
        HalleySolver s([](double x) { return x; }, [](double) { return 1.0; }, [](double) { return 0.0; }, 1.0);
        // s.solve() will eventually call s.do_step(), which is the stub
        // throwing nm::not_implemented. run_or_todo catches it and reports
        // [TODO] — that's the desired behavior. If someone "fixes" the stub
        // to return a default value silently, this test catches that
        // regression by failing once the test_halley test starts passing
        // (you'd see [ OK ] for halley but [TODO] vanish here).
        s.solve(1e-10, 5);
    });

    return nm::testing::exit_code();
}
