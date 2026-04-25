#include "ode.h"
#include "todo_helper.h"

#include <cmath>

using nm::testing::run_or_todo;
using namespace nm::ode;

// dy/dt = -y, y(0) = 1  →  exact y(t) = e^{-t}.
static RHS exp_decay() {
    return [](double, const State& y) { return State{-y[0]}; };
}

int main() {
    run_or_todo("euler exp-decay (loose)", []() {
        auto traj = euler(exp_decay(), 0.0, {1.0}, 1.0, 1e-3);
        NM_CHECK(!traj.t.empty());
        NM_CHECK_NEAR(traj.y.back()[0], std::exp(-1.0), 5e-4);
    });

    run_or_todo("rk4 exp-decay", []() {
        auto traj = rk4(exp_decay(), 0.0, {1.0}, 1.0, 1e-2);
        NM_CHECK_NEAR(traj.y.back()[0], std::exp(-1.0), 1e-8);
    });

    run_or_todo("rk45 exp-decay (adaptive)", []() {
        auto traj = rk45(exp_decay(), 0.0, {1.0}, 1.0, 1e-3, 1e-9);
        NM_CHECK_NEAR(traj.y.back()[0], std::exp(-1.0), 1e-8);
    });

    return nm::testing::exit_code();
}
