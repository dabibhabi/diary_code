#pragma once

#include <optional>
#include <string>
#include <utility>

#include "cuny_sms/domain/name.hpp"

namespace cuny_sms {

// A real person. Composes Name plus optional bio fields. The "private" stuff
// (SSN, DOB) is optional because the engine works without it; persistence layer
// may require it.
struct Person {
public:
    explicit Person(Name name) : name_(std::move(name)) {}

    Person(Name name, std::optional<std::string> dateOfBirth)
        : name_(std::move(name)), dateOfBirth_(std::move(dateOfBirth)) {}

    [[nodiscard]] const Name& name() const noexcept { return name_; }
    [[nodiscard]] const std::optional<std::string>& dateOfBirth() const noexcept {
        return dateOfBirth_;
    }

    // Convenience pass-throughs so callers can write person.first() without
    // person.name().first() chains. Composition shouldn't punish ergonomics.
    [[nodiscard]] std::string_view first() const noexcept { return name_.first(); }
    [[nodiscard]] std::string_view last() const noexcept { return name_.last(); }
    [[nodiscard]] std::string fullName() const { return name_.full(); }

    [[nodiscard]] friend bool operator==(const Person& a, const Person& b) noexcept = default;

private:
    Name name_;
    std::optional<std::string> dateOfBirth_;
};

}  // namespace cuny_sms
