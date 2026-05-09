#pragma once

#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>

namespace cuny_sms {

// Validated email wrapper. The actual format-from-rule logic lives in
// EmailGenerator (cuny_sms/domain/email_generator.hpp); this type is a strong typedef
// over std::string with a minimal invariant: contains exactly one '@'.
struct Email {
public:
    explicit Email(std::string value) : value_(std::move(value)) {
        const auto at = value_.find('@');
        if (at == std::string::npos || at == 0 || at == value_.size() - 1) {
            throw std::invalid_argument("Email: must contain exactly one '@'");
        }
        if (value_.find('@', at + 1) != std::string::npos) {
            throw std::invalid_argument("Email: multiple '@' symbols");
        }
    }

    [[nodiscard]] std::string_view value() const noexcept { return value_; }
    [[nodiscard]] const std::string& str() const noexcept { return value_; }

    [[nodiscard]] std::string_view localPart() const noexcept {
        return std::string_view{value_}.substr(0, value_.find('@'));
    }
    [[nodiscard]] std::string_view domain() const noexcept {
        return std::string_view{value_}.substr(value_.find('@') + 1);
    }

    [[nodiscard]] friend bool operator==(const Email& a, const Email& b) noexcept = default;

private:
    std::string value_;
};

}  // namespace cuny_sms
