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

// Teaching assistants — graduate students or advanced undergrads assisting in
// teaching duties. Distinct factory because hiring rules differ from faculty
// (often department-scoped, supervisor-required).
class TeachingAssistantFactory {
public:
    TeachingAssistantFactory(const TeachingAssistantFactory&) = delete;
    TeachingAssistantFactory& operator=(const TeachingAssistantFactory&) = delete;
    TeachingAssistantFactory(TeachingAssistantFactory&&) = delete;
    TeachingAssistantFactory& operator=(TeachingAssistantFactory&&) = delete;
    virtual ~TeachingAssistantFactory() = default;

    [[nodiscard]] virtual CampusCode campusCode() const noexcept = 0;
    [[nodiscard]] virtual Campus campus() const = 0;

    [[nodiscard]] virtual CunyMember assign(Person person, Department dept,
                                            std::string supervisor) const = 0;

protected:
    TeachingAssistantFactory() = default;
};

class CcnyTeachingAssistantFactory final : public TeachingAssistantFactory {
public:
    [[nodiscard]] CampusCode campusCode() const noexcept override { return CampusCode::CCNY; }
    [[nodiscard]] Campus campus() const override {
        return Campus{CampusCode::CCNY, "City College of New York", "Manhattan, NY"};
    }
    [[nodiscard]] CunyMember assign(Person person, Department dept,
                                    std::string supervisor) const override {
        return detail::buildMemberWithRole(
            std::move(person), campus(),
            std::make_unique<TeachingAssistantRole>(dept, std::move(supervisor)));
    }
};

class BrooklynTeachingAssistantFactory final : public TeachingAssistantFactory {
public:
    [[nodiscard]] CampusCode campusCode() const noexcept override { return CampusCode::Brooklyn; }
    [[nodiscard]] Campus campus() const override {
        return Campus{CampusCode::Brooklyn, "Brooklyn College", "Brooklyn, NY"};
    }
    [[nodiscard]] CunyMember assign(Person person, Department dept,
                                    std::string supervisor) const override {
        if (supervisor.empty()) {
            throw std::invalid_argument(
                "BrooklynTeachingAssistantFactory: supervisor must not be empty");
        }
        return detail::buildMemberWithRole(
            std::move(person), campus(),
            std::make_unique<TeachingAssistantRole>(dept, std::move(supervisor)));
    }
};

class HunterTeachingAssistantFactory final : public TeachingAssistantFactory {
public:
    [[nodiscard]] CampusCode campusCode() const noexcept override { return CampusCode::Hunter; }
    [[nodiscard]] Campus campus() const override {
        return Campus{CampusCode::Hunter, "Hunter College", "Manhattan, NY"};
    }
    [[nodiscard]] CunyMember assign(Person person, Department dept,
                                    std::string supervisor) const override {
        return detail::buildMemberWithRole(
            std::move(person), campus(),
            std::make_unique<TeachingAssistantRole>(dept, std::move(supervisor)));
    }
};

}  // namespace cuny_sms
