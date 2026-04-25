#include "pde.h"
#include "todo_helper.h"

#include <cmath>

using nm::testing::run_or_todo;
using namespace nm::pde;

// Initial: u(x,0) = sin(π x) on [0,1] with u=0 at both ends.
// Exact:   u(x,t) = sin(π x) * exp(-π^2 ν t).
static double max_error(const HeatGrid& g, double nu, double T) {
    double m = 0.0;
    for (size_t i = 0; i < g.x.size(); ++i) {
        double exact = std::sin(M_PI * g.x[i]) * std::exp(-M_PI * M_PI * nu * T);
        m            = std::max(m, std::abs(g.u[i] - exact));
    }
    return m;
}

int main() {
    const double nu = 0.1, T = 0.05;
    const int    N   = 64;
    auto         u0  = [](double x) { return std::sin(M_PI * x); };
    auto         bc0 = [](double) { return 0.0; };

    run_or_todo("heat_explicit converges", [&]() {
        auto g = heat_explicit(nu, 0.0, 1.0, T, N, 8000, u0, bc0, bc0);
        NM_CHECK(max_error(g, nu, T) < 5e-3);
    });

    run_or_todo("heat_implicit converges", [&]() {
        auto g = heat_implicit(nu, 0.0, 1.0, T, N, 200, u0, bc0, bc0);
        NM_CHECK(max_error(g, nu, T) < 5e-3);
    });

    run_or_todo("crank_nicolson converges (O(dt^2))", [&]() {
        auto g = crank_nicolson(nu, 0.0, 1.0, T, N, 50, u0, bc0, bc0);
        NM_CHECK(max_error(g, nu, T) < 5e-4);
    });

    return nm::testing::exit_code();
}
