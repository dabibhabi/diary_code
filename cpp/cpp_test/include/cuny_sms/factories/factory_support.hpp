#pragma once

#include <memory>
#include <utility>

#include "cuny_sms/domain/campus.hpp"
#include "cuny_sms/domain/cuny_member.hpp"
#include "cuny_sms/domain/person.hpp"
#include "cuny_sms/domain/role.hpp"

namespace cuny_sms::detail {

// Shared helper for the role-specific factories: build a CunyMember and
// attach a single role to it. Keeps the four factory hierarchies from each
// reimplementing the same three lines.
[[nodiscard]] inline CunyMember buildMemberWithRole(Person person, Campus campus,
                                                    std::unique_ptr<Role> role) {
    CunyMember m{std::move(person), std::move(campus)};
    m.addRole(std::move(role));
    return m;
}

}  // namespace cuny_sms::detail
