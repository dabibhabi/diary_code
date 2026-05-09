#pragma once

#include <algorithm>
#include <memory>
#include <utility>
#include <vector>

#include "cuny_sms/domain/campus.hpp"
#include "cuny_sms/domain/email.hpp"
#include "cuny_sms/domain/email_generator.hpp"
#include "cuny_sms/domain/employee_id.hpp"
#include "cuny_sms/domain/person.hpp"
#include "cuny_sms/domain/role.hpp"

namespace cuny_sms {

// A person attached to a campus, with a stable employeeId, a derived email,
// and a list of roles.
//
// Identity-stability rule (docs/01-overview.md): employeeId and email are
// computed once at construction and never recomputed.
//
// Composition over inheritance (docs/04-patterns.md): roles live in a
// List<Role> rather than CunyMember being a Student/Professor/etc. subclass.
// One member can hold multiple roles simultaneously (student + TA + admin).
class CunyMember {
public:
    CunyMember(Person person, Campus campus, EmployeeId emplid)
        : person_(std::move(person)), campus_(std::move(campus)), employeeId_(emplid),
          email_(EmailGenerator::generate(campus_.code(), person_, employeeId_)) {}

    CunyMember(Person person, Campus campus)
        : CunyMember(std::move(person), std::move(campus), EmployeeId{}) {}

    // Move-only — owning unique_ptrs in roles_ make copies non-trivial. If a
    // copy is genuinely needed, deep-clone via roles' clone().
    CunyMember(const CunyMember&) = delete;
    CunyMember& operator=(const CunyMember&) = delete;
    CunyMember(CunyMember&&) noexcept = default;
    CunyMember& operator=(CunyMember&&) noexcept = default;
    ~CunyMember() = default;

    [[nodiscard]] const Person& person() const noexcept { return person_; }
    [[nodiscard]] const Campus& campus() const noexcept { return campus_; }
    [[nodiscard]] EmployeeId employeeId() const noexcept { return employeeId_; }
    [[nodiscard]] const Email& email() const noexcept { return email_; }

    // Role composition --------------------------------------------------------
    void addRole(std::unique_ptr<Role> role) { roles_.push_back(std::move(role)); }

    [[nodiscard]] const std::vector<std::unique_ptr<Role>>& roles() const noexcept {
        return roles_;
    }

    [[nodiscard]] std::size_t roleCount() const noexcept { return roles_.size(); }

    [[nodiscard]] bool hasRole(RoleType type) const noexcept {
        return std::any_of(roles_.begin(), roles_.end(),
                           [type](const auto& r) { return r->type() == type; });
    }

    [[nodiscard]] const Role* firstRole(RoleType type) const noexcept {
        for (const auto& r : roles_) {
            if (r->type() == type)
                return r.get();
        }
        return nullptr;
    }

    // Returns true if a role of that type was found and removed.
    bool removeFirstRole(RoleType type) {
        const auto it = std::find_if(roles_.begin(), roles_.end(),
                                     [type](const auto& r) { return r->type() == type; });
        if (it == roles_.end())
            return false;
        roles_.erase(it);
        return true;
    }

private:
    Person person_;
    Campus campus_;
    EmployeeId employeeId_;
    Email email_;
    std::vector<std::unique_ptr<Role>> roles_;
};

}  // namespace cuny_sms
