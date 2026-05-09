#pragma once

#include <cstdint>
#include <string_view>

namespace cuny_sms {

// Stable identifiers for the campuses we recognize. Add via PR; do not parse
// from arbitrary strings at runtime.
enum class CampusCode : std::uint8_t {
    CCNY,
    Brooklyn,
    Hunter,
};

[[nodiscard]] constexpr std::string_view toString(CampusCode c) noexcept {
    switch (c) {
    case CampusCode::CCNY:
        return "ccny";
    case CampusCode::Brooklyn:
        return "brooklyn";
    case CampusCode::Hunter:
        return "hunter";
    }
    return "unknown";
}

[[nodiscard]] constexpr std::string_view emailDomainFor(CampusCode c) noexcept {
    switch (c) {
    case CampusCode::CCNY:
        return "ccny.cuny.edu";
    case CampusCode::Brooklyn:
        return "brooklyn.cuny.edu";
    case CampusCode::Hunter:
        return "hunter.cuny.edu";
    }
    return "cuny.edu";
}

}  // namespace cuny_sms
