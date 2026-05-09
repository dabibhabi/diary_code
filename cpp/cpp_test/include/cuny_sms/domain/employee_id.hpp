#pragma once

#include <cstdint>
#include <random>
#include <stdexcept>
#include <string>

namespace cuny_sms {

// 9-digit business-facing identifier. Stable for the lifetime of the member —
// constructed once, never regenerated.
//
// Replaces main.cpp's std::rand() usage with std::mt19937 (cf. docs/conventions/18-cpp-style.md).
struct EmployeeId {
public:
    static constexpr std::int32_t kMin = 100'000'000;
    static constexpr std::int32_t kMax = 999'999'999;

    // Random-init.
    EmployeeId() : value_(generate()) {}

    // Explicit-value constructor (used in tests and when loading from storage).
    explicit EmployeeId(std::int32_t value) : value_(value) {
        if (value < kMin || value > kMax) {
            throw std::invalid_argument("EmployeeId: value must be 9 digits");
        }
    }

    [[nodiscard]] std::int32_t value() const noexcept { return value_; }

    [[nodiscard]] std::string toString() const { return std::to_string(value_); }

    // Last N digits as a zero-padded string. Used by EmailGenerator.
    [[nodiscard]] std::string lastDigits(std::size_t n) const {
        auto s = toString();
        if (n >= s.size()) {
            return s;
        }
        return s.substr(s.size() - n);
    }

    [[nodiscard]] bool operator==(const EmployeeId& other) const noexcept = default;
    [[nodiscard]] auto operator<=>(const EmployeeId& other) const noexcept = default;

private:
    static std::int32_t generate() {
        // Per-thread engine, seeded once. Avoids std::rand's well-known quality
        // and thread-safety issues; see docs/conventions/18-cpp-style.md.
        static thread_local std::mt19937 rng{std::random_device{}()};
        std::uniform_int_distribution<std::int32_t> dist(kMin, kMax);
        return dist(rng);
    }

    std::int32_t value_;
};

}  // namespace cuny_sms
