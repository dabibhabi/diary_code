#pragma once

#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>

namespace cuny_sms {

// Strong-typed wrapper around a campus-specific course code such as
// "MATH 20100" (CCNY) or "MATH 1201" (Brooklyn). The format itself varies by
// campus — see the CourseFactory hierarchy. This class enforces only that the
// id is non-empty and contains no leading/trailing whitespace.
struct CourseId {
public:
    explicit CourseId(std::string value) : value_(std::move(value)) {
        if (value_.empty()) {
            throw std::invalid_argument("CourseId: must not be empty");
        }
        if (value_.front() == ' ' || value_.back() == ' ') {
            throw std::invalid_argument("CourseId: must not have leading/trailing whitespace");
        }
    }

    [[nodiscard]] std::string_view value() const noexcept { return value_; }
    [[nodiscard]] const std::string& str() const noexcept { return value_; }

    [[nodiscard]] friend bool operator==(const CourseId&, const CourseId&) noexcept = default;

private:
    std::string value_;
};

}  // namespace cuny_sms
