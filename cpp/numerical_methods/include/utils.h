#pragma once

#include "numerical_methods.h"

namespace nm::utils {

func derivative(func f, double h = 1e-6);

double integrate(func f, double a, double b, int n = 1000);

}  // namespace nm::utils
