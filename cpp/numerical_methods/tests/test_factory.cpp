#include "patterns/solver_factory.h"
#include "todo_helper.h"

#include <cmath>

using nm::testing::run_or_todo;
using namespace nm::patterns;

int main() {
    run_or_todo("Factory makes BISECTION", []() {
        SolverConfig cfg;
        cfg.f  = [](double x) { return x * x - 2.0; };
        cfg.a  = 0.0;
        cfg.b  = 2.0;
        auto s = SolverFactory::create(SolverKind::BISECTION, cfg);
        NM_CHECK(s != nullptr);
        auto r = s->solve(1e-10, 200);
        NM_CHECK(r.converged);
        NM_CHECK_NEAR(r.root, std::sqrt(2.0), 1e-9);
    });

    run_or_todo("Factory makes NEWTON", []() {
        SolverConfig cfg;
        cfg.f  = [](double x) { return x * x - 2.0; };
        cfg.df = [](double x) { return 2.0 * x; };
        cfg.x0 = 1.0;
        auto s = SolverFactory::create(SolverKind::NEWTON, cfg);
        NM_CHECK(s != nullptr);
        auto r = s->solve(1e-12, 200);
        NM_CHECK(r.converged);
        NM_CHECK_NEAR(r.root, std::sqrt(2.0), 1e-10);
    });

    return nm::testing::exit_code();
}
