#pragma once

#include <memory>
#include <optional>
#include <stdexcept>
#include <utility>

#include "cuny_sms/domain/admin_level.hpp"
#include "cuny_sms/domain/campus.hpp"
#include "cuny_sms/domain/campus_code.hpp"
#include "cuny_sms/domain/cuny_member.hpp"
#include "cuny_sms/domain/department.hpp"
#include "cuny_sms/factories/factory_support.hpp"
#include "cuny_sms/domain/person.hpp"
#include "cuny_sms/domain/role.hpp"

namespace cuny_sms {

// Admin members — IT, Registrar, Department Chair, Dean, Provost. Distinct
// factory because admin appointments have different (often campus-political)
// approval rules than student admissions or faculty hires.
class AdminFactory {
public:
    AdminFactory(const AdminFactory&) = delete;
    AdminFactory& operator=(const AdminFactory&) = delete;
    AdminFactory(AdminFactory&&) = delete;
    AdminFactory& operator=(AdminFactory&&) = delete;
    virtual ~AdminFactory() = default;

    [[nodiscard]] virtual CampusCode campusCode() const noexcept = 0;
    [[nodiscard]] virtual Campus campus() const = 0;

    // Appoint an admin. `scope` is the department the admin is responsible for
    // (only meaningful for DEPT_CHAIR and below); pass nullopt for system-wide
    // levels (DEAN, PROVOST).
    [[nodiscard]] virtual CunyMember
    appoint(Person person, AdminLevel level,
            std::optional<Department> scope = std::nullopt) const = 0;

protected:
    AdminFactory() = default;
};

class CcnyAdminFactory final : public AdminFactory {
public:
    [[nodiscard]] CampusCode campusCode() const noexcept override { return CampusCode::CCNY; }
    [[nodiscard]] Campus campus() const override {
        return Campus{CampusCode::CCNY, "City College of New York", "Manhattan, NY"};
    }
    [[nodiscard]] CunyMember
    appoint(Person person, AdminLevel level,
            std::optional<Department> scope = std::nullopt) const override {
        return detail::buildMemberWithRole(std::move(person), campus(),
                                           std::make_unique<AdminRole>(level, scope));
    }
};

class BrooklynAdminFactory final : public AdminFactory {
public:
    [[nodiscard]] CampusCode campusCode() const noexcept override { return CampusCode::Brooklyn; }
    [[nodiscard]] Campus campus() const override {
        return Campus{CampusCode::Brooklyn, "Brooklyn College", "Brooklyn, NY"};
    }
    [[nodiscard]] CunyMember
    appoint(Person person, AdminLevel level,
            std::optional<Department> scope = std::nullopt) const override {
        return detail::buildMemberWithRole(std::move(person), campus(),
                                           std::make_unique<AdminRole>(level, scope));
    }
};

class HunterAdminFactory final : public AdminFactory {
public:
    [[nodiscard]] CampusCode campusCode() const noexcept override { return CampusCode::Hunter; }
    [[nodiscard]] Campus campus() const override {
        return Campus{CampusCode::Hunter, "Hunter College", "Manhattan, NY"};
    }
    [[nodiscard]] CunyMember
    appoint(Person person, AdminLevel level,
            std::optional<Department> scope = std::nullopt) const override {
        // Hunter rule: only DEAN and PROVOST may be appointed without a scope.
        // IT/REGISTRAR/DEPT_CHAIR must be department-scoped.
        const bool requiresScope = (level == AdminLevel::IT || level == AdminLevel::Registrar ||
                                    level == AdminLevel::DeptChair);
        if (requiresScope && !scope.has_value()) {
            throw std::invalid_argument("HunterAdminFactory: IT/REGISTRAR/DEPT_CHAIR appointments "
                                        "require a department scope");
        }
        return detail::buildMemberWithRole(std::move(person), campus(),
                                           std::make_unique<AdminRole>(level, scope));
    }
};

}  // namespace cuny_sms
