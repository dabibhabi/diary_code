#pragma once

#include <cstdint>
#include <string_view>

namespace cuny_sms {

// docs/03-domain-model.md — admin role family.
enum class AdminLevel : std::uint8_t {
    IT,
    Registrar,
    DeptChair,
    Dean,
    Provost,
};

[[nodiscard]] constexpr std::string_view toString(AdminLevel a) noexcept {
    switch (a) {
    case AdminLevel::IT:
        return "IT";
    case AdminLevel::Registrar:
        return "REGISTRAR";
    case AdminLevel::DeptChair:
        return "DEPT_CHAIR";
    case AdminLevel::Dean:
        return "DEAN";
    case AdminLevel::Provost:
        return "PROVOST";
    }
    return "UNKNOWN";
}

}  // namespace cuny_sms
