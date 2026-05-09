#pragma once

#include <string>
#include <string_view>
#include <utility>

#include "cuny_sms/domain/campus_code.hpp"

namespace cuny_sms {

// A CUNY institution. Carries identity + the bits of config we need at
// Phase 1; the role-factory pointer arrives in Phase 2 (docs/03-domain-model.md).
struct Campus {
public:
    Campus(CampusCode code, std::string name, std::string location)
        : code_(code), name_(std::move(name)), location_(std::move(location)) {}

    [[nodiscard]] CampusCode code() const noexcept { return code_; }
    [[nodiscard]] std::string_view name() const noexcept { return name_; }
    [[nodiscard]] std::string_view location() const noexcept { return location_; }
    [[nodiscard]] std::string_view emailDomain() const noexcept { return emailDomainFor(code_); }

    [[nodiscard]] friend bool operator==(const Campus& a, const Campus& b) noexcept {
        return a.code_ == b.code_;  // identity == code; name/location are display only
    }

private:
    CampusCode code_;
    std::string name_;
    std::string location_;
};

}  // namespace cuny_sms
