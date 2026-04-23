#pragma once

#include <cmath>
#include <iostream>
#include <stdexcept>

inline int tests_passed = 0;
inline int tests_failed = 0;

#define TEST(name) static void name()

#define RUN(fn) do { \
    try { fn(); std::cout << "  PASS " << #fn << "\n"; ++tests_passed; } \
    catch (const std::exception& e) { std::cout << "  FAIL " << #fn << ": " << e.what() << "\n"; ++tests_failed; } \
    catch (...) { std::cout << "  FAIL " << #fn << " (unknown)\n"; ++tests_failed; } \
} while (0)

#define ASSERT_TRUE(expr) do { if (!(expr)) throw std::runtime_error("assertion failed: " #expr); } while (0)
#define ASSERT_EQ(a, b) do { if (!((a) == (b))) throw std::runtime_error("assertion failed: " #a " == " #b); } while (0)
#define ASSERT_NEAR(a, b, eps) do { if (std::abs((a) - (b)) > (eps)) throw std::runtime_error("assertion failed: " #a " ~= " #b); } while (0)
#define ASSERT_THROWS(expr) do { bool threw = false; try { (void)(expr); } catch (...) { threw = true; } if (!threw) throw std::runtime_error("expected throw: " #expr); } while (0)
