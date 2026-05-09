#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <utility>

#include "cuny_sms/domain/admin_level.hpp"
#include "cuny_sms/domain/department.hpp"

namespace cuny_sms {

// A "what hat is this person wearing" tag. The full hierarchy with per-role
// data is fleshed out in Phase 2 (docs/03-domain-model.md). This file gives the
// minimum to demo the abstract factories: each role type carries the bits the
// staff factories actually need to populate.
enum class RoleType : std::uint8_t {
    Student,
    Professor,
    TeachingAssistant,
    Admin,
};

[[nodiscard]] constexpr std::string_view toString(RoleType r) noexcept {
    switch (r) {
    case RoleType::Student:
        return "STUDENT";
    case RoleType::Professor:
        return "PROFESSOR";
    case RoleType::TeachingAssistant:
        return "TA";
    case RoleType::Admin:
        return "ADMIN";
    }
    return "UNKNOWN";
}

class Role {
public:
    explicit Role(RoleType type) : type_(type) {}
    Role(const Role&) = default;
    Role(Role&&) noexcept = default;
    Role& operator=(const Role&) = default;
    Role& operator=(Role&&) noexcept = default;
    virtual ~Role() = default;

    [[nodiscard]] RoleType type() const noexcept { return type_; }

    // Polymorphic clone — useful when the factories produce role pointers.
    [[nodiscard]] virtual std::unique_ptr<Role> clone() const = 0;

private:
    RoleType type_;
};

class StudentRole final : public Role {
public:
    StudentRole(Department major, double gpa = 0.0)
        : Role(RoleType::Student), major_(major), gpa_(gpa) {}

    [[nodiscard]] Department major() const noexcept { return major_; }
    [[nodiscard]] double gpa() const noexcept { return gpa_; }

    [[nodiscard]] std::unique_ptr<Role> clone() const override {
        return std::make_unique<StudentRole>(*this);
    }

private:
    Department major_;
    double gpa_;
};

class ProfessorRole final : public Role {
public:
    ProfessorRole(Department department, std::string specialization, bool tenured)
        : Role(RoleType::Professor), department_(department),
          specialization_(std::move(specialization)), tenured_(tenured) {}

    [[nodiscard]] Department department() const noexcept { return department_; }
    [[nodiscard]] std::string_view specialization() const noexcept { return specialization_; }
    [[nodiscard]] bool tenured() const noexcept { return tenured_; }

    [[nodiscard]] std::unique_ptr<Role> clone() const override {
        return std::make_unique<ProfessorRole>(*this);
    }

private:
    Department department_;
    std::string specialization_;
    bool tenured_;
};

class TeachingAssistantRole final : public Role {
public:
    TeachingAssistantRole(Department department, std::string supervisor)
        : Role(RoleType::TeachingAssistant), department_(department),
          supervisor_(std::move(supervisor)) {}

    [[nodiscard]] Department department() const noexcept { return department_; }
    [[nodiscard]] std::string_view supervisor() const noexcept { return supervisor_; }

    [[nodiscard]] std::unique_ptr<Role> clone() const override {
        return std::make_unique<TeachingAssistantRole>(*this);
    }

private:
    Department department_;
    std::string supervisor_;
};

class AdminRole final : public Role {
public:
    AdminRole(AdminLevel level, std::optional<Department> scope = std::nullopt)
        : Role(RoleType::Admin), level_(level), scope_(scope) {}

    [[nodiscard]] AdminLevel level() const noexcept { return level_; }
    [[nodiscard]] const std::optional<Department>& scope() const noexcept { return scope_; }

    [[nodiscard]] std::unique_ptr<Role> clone() const override {
        return std::make_unique<AdminRole>(*this);
    }

private:
    AdminLevel level_;
    std::optional<Department> scope_;
};

}  // namespace cuny_sms
