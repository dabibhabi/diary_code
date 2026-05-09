// CUNY SMS — Phase 1 demo.
//
// Exercises the identity types and the full split-factory hierarchy:
// StudentFactory / FacultyFactory / TeachingAssistantFactory / AdminFactory /
// CourseFactory / SectionFactory — all wired through the CampusServices facade.

#include <iostream>
#include <memory>
#include <vector>

#include "cuny_sms/identity.hpp"

using namespace cuny_sms;

namespace {

void printMember(const CunyMember& m, std::string_view label) {
    std::cout << "  " << label << ": " << m.person().fullName()
              << "  (id=" << m.employeeId().toString() << ", " << m.email().value()
              << ", roles=" << m.roleCount() << ")\n";
}

void demoSplitFactories(const CampusServices& cs) {
    std::cout << "\n[" << toString(cs.campusCode()) << "]\n";

    auto stu = cs.students().admit(Person{Name{"Ada", "Lovelace"}}, Department::Math, 3.95);
    auto prof =
        cs.faculty().hire(Person{Name{"Grace", "Hopper"}}, Department::CS, "Compilers", true);
    auto ta = cs.teachingAssistants().assign(Person{Name{"Alan", "Turing"}}, Department::CS,
                                             "Prof. Hopper");
    auto admin = (cs.campusCode() == CampusCode::Hunter)
                     ? cs.admins().appoint(Person{Name{"Mary", "Curie"}}, AdminLevel::IT,
                                           Department::Physics)
                     : cs.admins().appoint(Person{Name{"Mary", "Curie"}}, AdminLevel::IT);

    printMember(stu, "Student  ");
    printMember(prof, "Professor");
    printMember(ta, "TA       ");
    printMember(admin, "Admin    ");

    const CourseSpec calc1{Department::Math, 200, 1, "Calculus I", 120};
    const CourseSpec cs1{Department::CS, 100, 1, "Intro to CS", 150};
    const auto course1 = cs.courses().createCourse(calc1);
    const auto course2 = cs.courses().createCourse(cs1);
    std::cout << "  Courses : " << course1.id().str() << ", " << course2.id().str() << "\n";

    const auto section = cs.sections().createSection({
        course1,
        1,
        {MeetingSlot{DaySet::monWedFri(), 10 * 60, 11 * 60}},
        prof.employeeId(),
        40,
        cs.campusCode() == CampusCode::Hunter,  // honors only meaningful at Hunter
    });
    std::cout << "  Section : " << course1.id().str() << " section " << section.sectionId() << "\n";
}

void demoMultiRole() {
    std::cout << "\n=== Multi-role member (composition) ===\n";
    CunyMember john{Person{Name{"John", "Doe"}},
                    Campus{CampusCode::CCNY, "City College", "Manhattan"}};
    john.addRole(std::make_unique<StudentRole>(Department::CS, 3.5));
    john.addRole(std::make_unique<TeachingAssistantRole>(Department::CS, "Prof. Hopper"));
    john.addRole(std::make_unique<AdminRole>(AdminLevel::IT));
    printMember(john, "John     ");
    std::cout << "    student? " << john.hasRole(RoleType::Student) << "    TA? "
              << john.hasRole(RoleType::TeachingAssistant) << "    admin? "
              << john.hasRole(RoleType::Admin) << "    prof? " << john.hasRole(RoleType::Professor)
              << "\n";
}

}  // namespace

int main() {
    std::cout << std::boolalpha;
    std::cout << "=== Split factories per campus ===";

    for (auto code : {CampusCode::CCNY, CampusCode::Brooklyn, CampusCode::Hunter}) {
        demoSplitFactories(CampusServices::forCampus(code));
    }

    demoMultiRole();
    std::cout << "\nok.\n";
    return 0;
}
