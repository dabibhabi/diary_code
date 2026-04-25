#pragma once

#include <cstdint>
#include <functional>
#include <vector>

// Sampling and Monte-Carlo helpers. See TODO.md §7.
namespace nm::random_mc {

// Box-Muller — two U(0,1) -> two independent N(0,1).
// References: Wikipedia "Box-Muller_transform"; Glasserman §2.3.2.
struct NormalPair {
    double z1, z2;
};
NormalPair box_muller(double u1, double u2);

// Inverse standard-normal CDF — Φ^{-1}(u). Acklam's rational approximation
// reaches ~1e-15 accuracy. Required for Sobol-driven Monte Carlo because
// Box-Muller destroys low-discrepancy structure.
// References: Acklam's algorithm (search "Acklam inverse normal CDF").
double inv_normal_cdf(double u);

// 1-D Sobol low-discrepancy sequence. Generate the first n points of the
// 1-dimensional Sobol sequence (the Van der Corput base-2 sequence).
//   Higher dimensions need direction numbers (Joe & Kuo tables).
// References: Wikipedia "Sobol_sequence"; NR3 §7.8.
std::vector<double> sobol_1d(int n);

// Antithetic + control-variate variance reduction.
//   estimator: returns (sample_value, control_variate_value).
//   control_mean: known E[control].
// Returns (mean_estimate, stderr_estimate).
// References: Glasserman ch. 4.
struct MCResult {
    double mean;
    double stderr;
};
MCResult monte_carlo_with_control(std::function<std::pair<double, double>(uint64_t /*seed*/)> estimator,
                                  double control_mean, int n_paths, uint64_t seed = 42);

}  // namespace nm::random_mc
