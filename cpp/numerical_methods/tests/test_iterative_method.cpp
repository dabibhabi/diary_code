// Validates the Template Method skeleton in IterativeRootFinder::solve()
// AND the Observer fan-out in IRootFinder::notify(). Both must be
// implemented before this test passes.

#include "patterns/convergence_observer.h"
#include "patterns/solvers.h"
#include "todo_helper.h"

#include <cmath>
#include <memory>

using nm::testing::run_or_todo;
using namespace nm::patterns;

int main() {
    run_or_todo("solve() loop terminates on tolerance", []() {
        BisectionSolver s([](double x) { return x * x - 2.0; }, 0.0, 2.0);
        auto            r = s.solve(1e-10, 200);
        NM_CHECK(r.converged);
        NM_CHECK(static_cast<int>(r.errors.size()) == r.iterations);
    });

    run_or_todo("CapturingObserver records every iteration", []() {
        BisectionSolver s([](double x) { return x * x - 2.0; }, 0.0, 2.0);
        auto            cap = std::make_shared<CapturingObserver>();
        s.attach(cap);
        auto r = s.solve(1e-10, 200);
        NM_CHECK(r.converged);
        NM_CHECK(static_cast<int>(cap->events().size()) == r.iterations);
    });

    run_or_todo("EarlyStopObserver short-circuits the loop", []() {
        BisectionSolver s([](double x) { return x * x - 2.0; }, 0.0, 2.0);
        s.attach(std::make_shared<EarlyStopObserver>(1e-3));
        auto r = s.solve(1e-15, 200);  // very tight tolerance — would normally take ~50 iters
        NM_CHECK(r.iterations < 20);   // EarlyStop should fire well before that
    });

    return nm::testing::exit_code();
}
