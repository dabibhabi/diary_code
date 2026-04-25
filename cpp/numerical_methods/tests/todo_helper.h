#pragma once

#include "todo.h"

#include <cstdio>
#include <cstdlib>
#include <exception>
#include <functional>
#include <string>

namespace nm::testing {

// Track the worst outcome across all checks in a test binary.
inline int& exit_code() {
    static int code = 0;
    return code;
}

// Soft-assert: never aborts. Throws on failure so the surrounding
// run_or_todo() catches it and records the test as FAIL.
inline void check(bool ok, const std::string& msg) {
    if (!ok) throw std::runtime_error(msg);
}

// Run a single named check.
//   - If body() throws nm::not_implemented → mark TODO (CI still passes).
//   - If body() throws anything else → mark FAIL (CI fails).
//   - Otherwise → mark OK.
inline void run_or_todo(const char* name, std::function<void()> body) {
    try {
        body();
        std::printf("[ OK ] %s\n", name);
    } catch (const ::nm::not_implemented& e) { std::printf("[TODO] %s\n", name); } catch (const std::exception& e) {
        std::printf("[FAIL] %s: %s\n", name, e.what());
        exit_code() = 1;
    } catch (...) {
        std::printf("[FAIL] %s: unknown exception\n", name);
        exit_code() = 1;
    }
}

}  // namespace nm::testing

#define NM_CHECK(expr) \
    ::nm::testing::check((expr), std::string(#expr) + " failed at " + __FILE__ + ":" + std::to_string(__LINE__))

#define NM_CHECK_NEAR(a, b, tol)                                                                                 \
    ::nm::testing::check(std::abs((a) - (b)) < (tol), std::string(#a " ~= " #b " failed at ") + __FILE__ + ":" + \
                                                          std::to_string(__LINE__) + " (|diff| > " #tol ")")
