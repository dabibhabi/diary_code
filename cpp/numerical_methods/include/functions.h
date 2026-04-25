#pragma once

#include "numerical_methods.h"

#include <string>

namespace nm {

struct TestFunction {
    std::string name;
    func        f;
    double      a;
    double      b;
    double      x0;
    double      x1;
};

const std::vector<TestFunction>& test_functions();

}  // namespace nm
