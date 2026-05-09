#pragma once

#include <memory>
#include <utility>

#include "cuny_sms/domain/campus.hpp"
#include "cuny_sms/domain/campus_code.hpp"
#include "cuny_sms/domain/cuny_member.hpp"
#include "cuny_sms/domain/department.hpp"
#include "cuny_sms/factories/factory_support.hpp"
#include "cuny_sms/domain/person.hpp"
#include "cuny_sms/domain/role.hpp"

namespace cuny_sms {

// Abstract factory for student records. Each campus may apply its own
// admission rules, default GPA values, or transfer-credit handling.
class StudentFactory {
public:
    StudentFactory(const StudentFactory&) = delete;
    StudentFactory& operator=(const StudentFactory&) = delete;
    StudentFactory(StudentFactory&&) = delete;
    StudentFactory& operator=(StudentFactory&&) = delete;
    virtual ~StudentFactory() = default;

    [[nodiscard]] virtual CampusCode campusCode() const noexcept = 0;
    [[nodiscard]] virtual Campus campus() const = 0;

    // Standard admit.
    [[nodiscard]] virtual CunyMember admit(Person person, Department major,
                                           double gpa = 0.0) const = 0;

protected:
    StudentFactory() = default;
};

class CcnyStudentFactory final : public StudentFactory {
public:
    [[nodiscard]] CampusCode campusCode() const noexcept override { return CampusCode::CCNY; }
    [[nodiscard]] Campus campus() const override {
        return Campus{CampusCode::CCNY, "City College of New York", "Manhattan, NY"};
    }
    [[nodiscard]] CunyMember admit(Person person, Department major,
                                   double gpa = 0.0) const override {
        return detail::buildMemberWithRole(std::move(person), campus(),
                                           std::make_unique<StudentRole>(major, gpa));
    }
};

class BrooklynStudentFactory final : public StudentFactory {
public:
    [[nodiscard]] CampusCode campusCode() const noexcept override { return CampusCode::Brooklyn; }
    [[nodiscard]] Campus campus() const override {
        return Campus{CampusCode::Brooklyn, "Brooklyn College", "Brooklyn, NY"};
    }
    [[nodiscard]] CunyMember admit(Person person, Department major,
                                   double gpa = 0.0) const override {
        return detail::buildMemberWithRole(std::move(person), campus(),
                                           std::make_unique<StudentRole>(major, gpa));
    }
};

class HunterStudentFactory final : public StudentFactory {
public:
    [[nodiscard]] CampusCode campusCode() const noexcept override { return CampusCode::Hunter; }
    [[nodiscard]] Campus campus() const override {
        return Campus{CampusCode::Hunter, "Hunter College", "Manhattan, NY"};
    }
    [[nodiscard]] CunyMember admit(Person person, Department major,
                                   double gpa = 0.0) const override {
        return detail::buildMemberWithRole(std::move(person), campus(),
                                           std::make_unique<StudentRole>(major, gpa));
    }
};

}  // namespace cuny_sms
