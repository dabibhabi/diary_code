#pragma once

#include <cstdint>
#include <string_view>

namespace cuny_sms {

// First-build catalog scope: docs/03-domain-model.md
enum class Department : std::uint8_t {
    Math,
    CS,
    Stats,
    Physics,
};

[[nodiscard]] constexpr std::string_view toString(Department d) noexcept {
    switch (d) {
    case Department::Math:
        return "MATH";
    case Department::CS:
        return "CS";
    case Department::Stats:
        return "STATS";
    case Department::Physics:
        return "PHYS";
    }
    return "UNKNOWN";
}

}  // namespace cuny_sms
