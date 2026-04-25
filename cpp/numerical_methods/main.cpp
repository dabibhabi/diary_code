// Runner: drives every test function through the SolverFactory and prints
// per-method convergence summaries via a CapturingObserver. Once you've
// implemented IterativeRootFinder::solve() + the observer + the factory,
// this binary becomes a friendly demo of the OO API.

#include "functions.h"
#include "patterns/convergence_observer.h"
#include "patterns/solver_factory.h"

#include <cstdio>
#include <memory>
#include <stdexcept>

using namespace nm;
using namespace nm::patterns;

namespace {

const char* kind_name(SolverKind k) {
    switch (k) {
        case SolverKind::BISECTION: return "bisection";
        case SolverKind::NEWTON: return "newton";
        case SolverKind::SECANT: return "secant";
        case SolverKind::BRENT: return "brent";
        case SolverKind::HALLEY: return "halley";
        case SolverKind::RIDDERS: return "ridders";
        case SolverKind::MULLER: return "muller";
        case SolverKind::CHANDRUPATLA: return "chandrupatla";
    }
    return "?";
}

void try_run(SolverKind k, const SolverConfig& cfg) {
    try {
        auto solver = SolverFactory::create(k, cfg);
        if (!solver) {
            std::printf("  %-12s | factory returned nullptr\n", kind_name(k));
            return;
        }
        auto cap = std::make_shared<CapturingObserver>();
        solver->attach(cap);
        const auto r = solver->solve(1e-12, 200);
        std::printf("  %-12s | iters=%3d | root=% .12f | order~%.3f | %s\n", kind_name(k), r.iterations, r.root,
                    estimate_order(r.errors), r.converged ? "converged" : "DID NOT converge");
    } catch (const std::exception& e) { std::printf("  %-12s | %s\n", kind_name(k), e.what()); }
}

}  // namespace

int main() {
    for (const auto& tf : test_functions()) {
        std::printf("\nf(x) = %s\n", tf.name.c_str());

        SolverConfig cfg;
        cfg.f  = tf.f;
        cfg.df = [f = tf.f](double x) {
            const double h = 1e-6;
            return (f(x + h) - f(x - h)) / (2.0 * h);
        };
        cfg.d2f = [f = tf.f](double x) {
            const double h = 1e-4;
            return (f(x + h) - 2.0 * f(x) + f(x - h)) / (h * h);
        };
        cfg.a  = tf.a;
        cfg.b  = tf.b;
        cfg.x0 = tf.x0;
        cfg.x1 = tf.x1;
        cfg.x2 = 0.5 * (tf.x0 + tf.x1);

        for (auto k : {SolverKind::BISECTION, SolverKind::NEWTON, SolverKind::SECANT, SolverKind::BRENT,
                       SolverKind::HALLEY, SolverKind::RIDDERS, SolverKind::MULLER, SolverKind::CHANDRUPATLA}) {
            try_run(k, cfg);
        }
    }
    return 0;
}
