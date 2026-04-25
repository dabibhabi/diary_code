#pragma once

// Factory pattern (creational) — constructs an IRootFinder by enum.
//
// Why it fits: in calibration code you often A/B test methods. A Factory lets
// `for (auto kind : {BISECTION, NEWTON, BRENT}) ...` drive the comparison
// without per-method constructor noise.

#include "iterative_root_finder.h"
#include "numerical_methods.h"

#include <memory>

namespace nm::patterns {

enum class SolverKind {
    BISECTION,
    NEWTON,
    SECANT,
    BRENT,
    HALLEY,
    RIDDERS,
    MULLER,
    CHANDRUPATLA,
};

// Most kinds need a function and a bracket OR a starting point. Different
// kinds need different inputs, so we use a small config struct rather than
// many overloads. Fields not relevant to the chosen kind are ignored.
struct SolverConfig {
    func   f;
    func   df;   // newton, halley
    func   d2f;  // halley
    double a  = 0.0;
    double b  = 0.0;
    double x0 = 0.0;
    double x1 = 0.0;
    double x2 = 0.0;  // muller
};

class SolverFactory {
public:
    // Returns nullptr if the config is invalid for the chosen kind
    // (e.g. NEWTON without df, BISECTION with non-bracketing a/b).
    // Throws nm::not_implemented while still a stub.
    static std::unique_ptr<IRootFinder> create(SolverKind kind, const SolverConfig& cfg);  // STUB
};

}  // namespace nm::patterns
