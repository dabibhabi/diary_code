#pragma once

#include <memory>
#include <stdexcept>
#include <string>
#include <utility>

#include "cuny_sms/domain/campus.hpp"
#include "cuny_sms/domain/campus_code.hpp"
#include "cuny_sms/domain/cuny_member.hpp"
#include "cuny_sms/domain/department.hpp"
#include "cuny_sms/factories/factory_support.hpp"
#include "cuny_sms/domain/person.hpp"
#include "cuny_sms/domain/role.hpp"

namespace cuny_sms {

// "Faculty" = professors (tenure-track, tenured, or otherwise teaching faculty).
// Adjuncts could be modeled as a subtype later; for Phase 1 they all share
// ProfessorRole.
class FacultyFactory {
public:
    FacultyFactory(const FacultyFactory&) = delete;
    FacultyFactory& operator=(const FacultyFactory&) = delete;
    FacultyFactory(FacultyFactory&&) = delete;
    FacultyFactory& operator=(FacultyFactory&&) = delete;
    virtual ~FacultyFactory() = default;

    [[nodiscard]] virtual CampusCode campusCode() const noexcept = 0;
    [[nodiscard]] virtual Campus campus() const = 0;

    [[nodiscard]] virtual CunyMember hire(Person person, Department dept,
                                          std::string specialization, bool tenured) const = 0;

protected:
    FacultyFactory() = default;
};

class CcnyFacultyFactory final : public FacultyFactory {
public:
    [[nodiscard]] CampusCode campusCode() const noexcept override { return CampusCode::CCNY; }
    [[nodiscard]] Campus campus() const override {
        return Campus{CampusCode::CCNY, "City College of New York", "Manhattan, NY"};
    }
    [[nodiscard]] CunyMember hire(Person person, Department dept, std::string specialization,
                                  bool tenured) const override {
        return detail::buildMemberWithRole(
            std::move(person), campus(),
            std::make_unique<ProfessorRole>(dept, std::move(specialization), tenured));
    }
};

class BrooklynFacultyFactory final : public FacultyFactory {
public:
    [[nodiscard]] CampusCode campusCode() const noexcept override { return CampusCode::Brooklyn; }
    [[nodiscard]] Campus campus() const override {
        return Campus{CampusCode::Brooklyn, "Brooklyn College", "Brooklyn, NY"};
    }
    [[nodiscard]] CunyMember hire(Person person, Department dept, std::string specialization,
                                  bool tenured) const override {
        // Brooklyn requires a stated specialization on every faculty hire.
        if (specialization.empty()) {
            throw std::invalid_argument("BrooklynFacultyFactory: specialization must not be empty");
        }
        return detail::buildMemberWithRole(
            std::move(person), campus(),
            std::make_unique<ProfessorRole>(dept, std::move(specialization), tenured));
    }
};

class HunterFacultyFactory final : public FacultyFactory {
public:
    [[nodiscard]] CampusCode campusCode() const noexcept override { return CampusCode::Hunter; }
    [[nodiscard]] Campus campus() const override {
        return Campus{CampusCode::Hunter, "Hunter College", "Manhattan, NY"};
    }
    [[nodiscard]] CunyMember hire(Person person, Department dept, std::string specialization,
                                  bool tenured) const override {
        return detail::buildMemberWithRole(
            std::move(person), campus(),
            std::make_unique<ProfessorRole>(dept, std::move(specialization), tenured));
    }
};

}  // namespace cuny_sms
