#pragma once

#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>

namespace cuny_sms {

// First/last name pair. Names can change (marriage, legal); identity stability
// is the responsibility of EmployeeId, not Name.
//
// Modeled as its own value type rather than two raw strings on Person because:
// - composition over inheritance: Person *has-a* Name
// - the formatting helpers (fullName, initials, sortKey) belong to Name itself
// - Name shows up independently in displays where the rest of Person is overkill
struct Name {
public:
    Name(std::string first, std::string last) : first_(std::move(first)), last_(std::move(last)) {
        if (first_.empty()) {
            throw std::invalid_argument("Name: first must not be empty");
        }
        if (last_.empty()) {
            throw std::invalid_argument("Name: last must not be empty");
        }
    }

    [[nodiscard]] std::string_view first() const noexcept { return first_; }
    [[nodiscard]] std::string_view last() const noexcept { return last_; }

    [[nodiscard]] std::string full() const { return first_ + " " + last_; }
    [[nodiscard]] std::string initials() const {
        return std::string{first_.front()} + "." + std::string{last_.front()};
    }
    // "Doe, John" — the canonical sort form.
    [[nodiscard]] std::string sortKey() const { return last_ + ", " + first_; }

    [[nodiscard]] friend bool operator==(const Name& a, const Name& b) noexcept = default;

private:
    std::string first_;
    std::string last_;
};

}  // namespace cuny_sms
