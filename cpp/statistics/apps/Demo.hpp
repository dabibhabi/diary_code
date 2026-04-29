#pragma once

#include "Dataset.hpp"

namespace stats::demo {

// Each function prints one section to std::cout.
// `run_all` wires them together with the project's sample data.
void run_descriptive_stats(const Dataset& data);
void run_order_stats(const Dataset& data);
void run_mle();
void run_inference(const Dataset& data);

void run_all();

}  // namespace stats::demo
