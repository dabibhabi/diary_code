#pragma once

#include <memory>
#include <stdexcept>

#include "cuny_sms/factories/admin_factory.hpp"
#include "cuny_sms/domain/campus_code.hpp"
#include "cuny_sms/factories/course_factory.hpp"
#include "cuny_sms/factories/faculty_factory.hpp"
#include "cuny_sms/factories/section_factory.hpp"
#include "cuny_sms/factories/student_factory.hpp"
#include "cuny_sms/factories/teaching_assistant_factory.hpp"

namespace cuny_sms {

// Facade that bundles every per-campus factory into one place. Callers ask
// "give me the CCNY services" and receive concrete instances for student,
// faculty, TA, admin, course, and section creation — all from the same campus
// family.
//
// This is the GoF "Abstract Factory" applied at the *campus* level: one
// CampusServices is a coherent family of related factories.
class CampusServices {
public:
    CampusServices(std::unique_ptr<StudentFactory> students,
                   std::unique_ptr<FacultyFactory> faculty,
                   std::unique_ptr<TeachingAssistantFactory> tas,
                   std::unique_ptr<AdminFactory> admins, std::unique_ptr<CourseFactory> courses,
                   std::unique_ptr<SectionFactory> sections)
        : students_(std::move(students)), faculty_(std::move(faculty)), tas_(std::move(tas)),
          admins_(std::move(admins)), courses_(std::move(courses)), sections_(std::move(sections)) {
    }

    [[nodiscard]] CampusCode campusCode() const noexcept { return students_->campusCode(); }

    [[nodiscard]] const StudentFactory& students() const noexcept { return *students_; }
    [[nodiscard]] const FacultyFactory& faculty() const noexcept { return *faculty_; }
    [[nodiscard]] const TeachingAssistantFactory& teachingAssistants() const noexcept {
        return *tas_;
    }
    [[nodiscard]] const AdminFactory& admins() const noexcept { return *admins_; }
    [[nodiscard]] const CourseFactory& courses() const noexcept { return *courses_; }
    [[nodiscard]] const SectionFactory& sections() const noexcept { return *sections_; }

    // Convenience constructors for the three CUNY campuses we model. Returns a
    // CampusServices wired with the matching concrete factories.
    [[nodiscard]] static CampusServices forCampus(CampusCode code);

private:
    std::unique_ptr<StudentFactory> students_;
    std::unique_ptr<FacultyFactory> faculty_;
    std::unique_ptr<TeachingAssistantFactory> tas_;
    std::unique_ptr<AdminFactory> admins_;
    std::unique_ptr<CourseFactory> courses_;
    std::unique_ptr<SectionFactory> sections_;
};

inline CampusServices CampusServices::forCampus(CampusCode code) {
    switch (code) {
    case CampusCode::CCNY:
        return CampusServices{
            std::make_unique<CcnyStudentFactory>(),
            std::make_unique<CcnyFacultyFactory>(),
            std::make_unique<CcnyTeachingAssistantFactory>(),
            std::make_unique<CcnyAdminFactory>(),
            std::make_unique<CcnyCourseFactory>(),
            std::make_unique<CcnySectionFactory>(),
        };
    case CampusCode::Brooklyn:
        return CampusServices{
            std::make_unique<BrooklynStudentFactory>(),
            std::make_unique<BrooklynFacultyFactory>(),
            std::make_unique<BrooklynTeachingAssistantFactory>(),
            std::make_unique<BrooklynAdminFactory>(),
            std::make_unique<BrooklynCourseFactory>(),
            std::make_unique<BrooklynSectionFactory>(),
        };
    case CampusCode::Hunter:
        return CampusServices{
            std::make_unique<HunterStudentFactory>(),
            std::make_unique<HunterFacultyFactory>(),
            std::make_unique<HunterTeachingAssistantFactory>(),
            std::make_unique<HunterAdminFactory>(),
            std::make_unique<HunterCourseFactory>(),
            std::make_unique<HunterSectionFactory>(),
        };
    }
    throw std::invalid_argument("CampusServices::forCampus: unknown CampusCode");
}

}  // namespace cuny_sms
