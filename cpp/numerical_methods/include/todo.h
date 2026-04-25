#pragma once

#include <stdexcept>
#include <string>

namespace nm {

// Thrown by stubbed methods so tests can distinguish "not yet implemented"
// (TODO — informational) from "implemented but wrong" (real failure).
struct not_implemented : std::runtime_error {
    using std::runtime_error::runtime_error;
};

}  // namespace nm

#define NM_TODO(name) throw ::nm::not_implemented(std::string("TODO: ") + (name))
