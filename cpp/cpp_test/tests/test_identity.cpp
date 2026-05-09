#include <catch2/catch_test_macros.hpp>

#include <stdexcept>
#include <string>

#include "cuny_sms/identity.hpp"

using namespace cuny_sms;  // NOLINT(google-build-using-namespace) — test scope only

// ===========================================================================
// Identity primitives
// ===========================================================================
TEST_CASE("Name composes first/last and produces formatted forms", "[identity][name]") {
    Name n{"John", "Doe"};
    REQUIRE(n.first() == "John");
    REQUIRE(n.last() == "Doe");
    REQUIRE(n.full() == "John Doe");
    REQUIRE(n.initials() == "J.D");
    REQUIRE(n.sortKey() == "Doe, John");
}

TEST_CASE("Name rejects empty inputs", "[identity][name]") {
    REQUIRE_THROWS_AS(Name("", "Doe"), std::invalid_argument);
    REQUIRE_THROWS_AS(Name("John", ""), std::invalid_argument);
}

TEST_CASE("EmployeeId stays stable across a member's lifetime", "[identity][emplid]") {
    EmployeeId a;
    EmployeeId b{a.value()};
    REQUIRE(a == b);
    REQUIRE(a.value() == b.value());
    REQUIRE(a.toString() == b.toString());
    REQUIRE(a.lastDigits(3) == b.lastDigits(3));
}

TEST_CASE("EmployeeId is always 9 digits when generated", "[identity][emplid]") {
    for (int i = 0; i < 32; ++i) {
        EmployeeId id;
        REQUIRE(id.value() >= EmployeeId::kMin);
        REQUIRE(id.value() <= EmployeeId::kMax);
        REQUIRE(id.toString().size() == 9);
    }
}

TEST_CASE("EmployeeId rejects out-of-range explicit values", "[identity][emplid]") {
    REQUIRE_THROWS_AS(EmployeeId{0}, std::invalid_argument);
    REQUIRE_THROWS_AS(EmployeeId{99'999'999}, std::invalid_argument);
    REQUIRE_THROWS_AS(EmployeeId{1'000'000'000}, std::invalid_argument);
}

TEST_CASE("EmailGenerator follows campus-aware format", "[identity][email]") {
    Person person{Name{"John", "Smith"}};
    EmployeeId emplid{546'792'341};
    const Email e = EmailGenerator::generate(CampusCode::CCNY, person, emplid);
    REQUIRE(e.value() == "ccny_jsmith341@ccny.cuny.edu");
}

TEST_CASE("EmailGenerator differs by campus", "[identity][email]") {
    Person person{Name{"Ada", "Lovelace"}};
    EmployeeId emplid{123'456'789};
    REQUIRE(EmailGenerator::generate(CampusCode::CCNY, person, emplid).value() ==
            "ccny_alovelace789@ccny.cuny.edu");
    REQUIRE(EmailGenerator::generate(CampusCode::Brooklyn, person, emplid).value() ==
            "brooklyn_alovelace789@brooklyn.cuny.edu");
    REQUIRE(EmailGenerator::generate(CampusCode::Hunter, person, emplid).value() ==
            "hunter_alovelace789@hunter.cuny.edu");
}

TEST_CASE("Email rejects malformed values", "[identity][email]") {
    REQUIRE_THROWS_AS(Email{"no_at_sign"}, std::invalid_argument);
    REQUIRE_THROWS_AS(Email{"@leading"}, std::invalid_argument);
    REQUIRE_THROWS_AS(Email{"trailing@"}, std::invalid_argument);
    REQUIRE_THROWS_AS(Email{"two@@signs.com"}, std::invalid_argument);
}

TEST_CASE("CunyMember caches email — does not regenerate on every call",
          "[identity][cuny_member][regression]") {
    CunyMember m{Person{Name{"John", "Doe"}}, Campus{CampusCode::CCNY, "City College", "Manhattan"},
                 EmployeeId{200'000'042}};
    REQUIRE(m.email().value() == "ccny_jdoe042@ccny.cuny.edu");
    REQUIRE(m.email().value() == m.email().value());
}

TEST_CASE("CunyMember composes multiple roles simultaneously",
          "[identity][cuny_member][composition]") {
    CunyMember m{Person{Name{"Carol", "Tang"}}, Campus{CampusCode::Hunter, "Hunter", "Manhattan"}};
    m.addRole(std::make_unique<StudentRole>(Department::CS, 3.7));
    m.addRole(std::make_unique<TeachingAssistantRole>(Department::CS, "Prof. Hopper"));
    m.addRole(std::make_unique<AdminRole>(AdminLevel::IT, Department::CS));

    REQUIRE(m.roleCount() == 3);
    REQUIRE(m.hasRole(RoleType::Student));
    REQUIRE(m.hasRole(RoleType::TeachingAssistant));
    REQUIRE(m.hasRole(RoleType::Admin));
    REQUIRE_FALSE(m.hasRole(RoleType::Professor));

    REQUIRE(m.removeFirstRole(RoleType::TeachingAssistant));
    REQUIRE_FALSE(m.hasRole(RoleType::TeachingAssistant));
    REQUIRE(m.roleCount() == 2);
}

// ===========================================================================
// Split factory hierarchies
// ===========================================================================
TEST_CASE("StudentFactory admits students per campus", "[factory][student]") {
    CcnyStudentFactory ccny;
    BrooklynStudentFactory brooklyn;
    HunterStudentFactory hunter;

    auto a = ccny.admit(Person{Name{"X", "Y"}}, Department::Math, 3.8);
    auto b = brooklyn.admit(Person{Name{"X", "Y"}}, Department::CS, 3.5);
    auto c = hunter.admit(Person{Name{"X", "Y"}}, Department::Physics);

    REQUIRE(a.campus().code() == CampusCode::CCNY);
    REQUIRE(b.campus().code() == CampusCode::Brooklyn);
    REQUIRE(c.campus().code() == CampusCode::Hunter);
    REQUIRE(a.hasRole(RoleType::Student));
    REQUIRE(b.hasRole(RoleType::Student));
    REQUIRE(c.hasRole(RoleType::Student));
}

TEST_CASE("FacultyFactory enforces campus-specific hire rules", "[factory][faculty]") {
    CcnyFacultyFactory ccny;
    BrooklynFacultyFactory brooklyn;
    HunterFacultyFactory hunter;

    // CCNY accepts empty specialization.
    REQUIRE_NOTHROW(ccny.hire(Person{Name{"X", "Y"}}, Department::CS, "", false));
    // Brooklyn requires specialization.
    REQUIRE_THROWS_AS(brooklyn.hire(Person{Name{"X", "Y"}}, Department::CS, "", false),
                      std::invalid_argument);
    REQUIRE_NOTHROW(brooklyn.hire(Person{Name{"X", "Y"}}, Department::CS, "Compilers", true));
    // Hunter allows everything.
    REQUIRE_NOTHROW(hunter.hire(Person{Name{"X", "Y"}}, Department::Physics, "Astrophysics", true));
}

TEST_CASE("TeachingAssistantFactory enforces supervisor rules", "[factory][ta]") {
    CcnyTeachingAssistantFactory ccny;
    BrooklynTeachingAssistantFactory brooklyn;
    HunterTeachingAssistantFactory hunter;

    REQUIRE_NOTHROW(ccny.assign(Person{Name{"X", "Y"}}, Department::CS, ""));
    REQUIRE_THROWS_AS(brooklyn.assign(Person{Name{"X", "Y"}}, Department::CS, ""),
                      std::invalid_argument);
    REQUIRE_NOTHROW(brooklyn.assign(Person{Name{"X", "Y"}}, Department::CS, "Prof. Smith"));
    REQUIRE_NOTHROW(hunter.assign(Person{Name{"X", "Y"}}, Department::CS, ""));
}

TEST_CASE("AdminFactory enforces Hunter scope rule", "[factory][admin]") {
    CcnyAdminFactory ccny;
    BrooklynAdminFactory brooklyn;
    HunterAdminFactory hunter;

    // CCNY/Brooklyn: any level, optional scope.
    REQUIRE_NOTHROW(ccny.appoint(Person{Name{"X", "Y"}}, AdminLevel::IT));
    REQUIRE_NOTHROW(brooklyn.appoint(Person{Name{"X", "Y"}}, AdminLevel::Provost));

    // Hunter: IT/REGISTRAR/DEPT_CHAIR require scope; DEAN/PROVOST do not.
    REQUIRE_THROWS_AS(hunter.appoint(Person{Name{"X", "Y"}}, AdminLevel::IT),
                      std::invalid_argument);
    REQUIRE_NOTHROW(hunter.appoint(Person{Name{"X", "Y"}}, AdminLevel::IT, Department::CS));
    REQUIRE_NOTHROW(hunter.appoint(Person{Name{"X", "Y"}}, AdminLevel::Provost));
    REQUIRE_NOTHROW(hunter.appoint(Person{Name{"X", "Y"}}, AdminLevel::Dean));
}

// ===========================================================================
// CourseFactory + SectionFactory
// ===========================================================================
TEST_CASE("CourseFactory naming differs per campus", "[factory][course]") {
    CcnyCourseFactory ccny;
    BrooklynCourseFactory brooklyn;
    HunterCourseFactory hunter;

    const CourseSpec calc1{Department::Math, 200, 1, "Calculus I", 120};
    REQUIRE(ccny.createCourse(calc1).id().str() == "MATH 20100");
    REQUIRE(brooklyn.createCourse(calc1).id().str() == "MATH 2001");
    REQUIRE(hunter.createCourse(calc1).id().str() == "MATH 201");

    const CourseSpec cs1{Department::CS, 100, 27, "Intro to CS", 150};
    REQUIRE(ccny.createCourse(cs1).id().str() == "CSC 12700");
    REQUIRE(brooklyn.createCourse(cs1).id().str() == "CISC 1027");
    REQUIRE(hunter.createCourse(cs1).id().str() == "CSCI 127");
}

TEST_CASE("SectionFactory naming differs per campus", "[factory][section]") {
    CcnyCourseFactory cf;
    auto course = cf.createCourse({Department::CS, 200, 1, "Algorithms", 100});

    CcnySectionFactory ccny;
    BrooklynSectionFactory brooklyn;
    HunterSectionFactory hunter;

    const std::vector<MeetingSlot> mwf{MeetingSlot{DaySet::monWedFri(), 10 * 60, 11 * 60}};
    const EmployeeId prof{500'000'001};

    auto a = ccny.createSection({course, 1, mwf, prof, 40, false});
    auto b = brooklyn.createSection({course, 1, mwf, prof, 40, false});
    auto c = hunter.createSection({course, 1, mwf, prof, 40, false});
    auto d = hunter.createSection({course, 1, mwf, prof, 40, true});  // honors

    REQUIRE(a.sectionId() == "01");
    REQUIRE(b.sectionId() == "TY1");
    REQUIRE(c.sectionId() == "01");
    REQUIRE(d.sectionId() == "01H");
}

TEST_CASE("Section conflict detection via MeetingSlot overlap", "[factory][section][conflict]") {
    CcnyCourseFactory cf;
    auto course1 = cf.createCourse({Department::Math, 200, 1, "Calc I", 100});
    auto course2 = cf.createCourse({Department::CS, 200, 1, "Algo", 100});

    CcnySectionFactory sf;
    EmployeeId prof{500'000'001};

    auto s1 = sf.createSection(
        {course1, 1, {MeetingSlot{DaySet::monWedFri(), 10 * 60, 11 * 60}}, prof, 40, false});
    auto s2 = sf.createSection({course2,
                                1,
                                {MeetingSlot{DaySet::monWedFri(), 10 * 60 + 30, 11 * 60 + 30}},
                                prof,
                                40,
                                false});
    auto s3 = sf.createSection(
        {course2, 2, {MeetingSlot{DaySet::tueThu(), 10 * 60, 11 * 60 + 30}}, prof, 40, false});

    REQUIRE(s1.conflictsWith(s2));        // same days, overlapping time
    REQUIRE_FALSE(s1.conflictsWith(s3));  // different days, no conflict
}

// ===========================================================================
// CampusServices facade
// ===========================================================================
TEST_CASE("CampusServices wires the right family of factories", "[factory][campus_services]") {
    auto ccny = CampusServices::forCampus(CampusCode::CCNY);
    auto brooklyn = CampusServices::forCampus(CampusCode::Brooklyn);
    auto hunter = CampusServices::forCampus(CampusCode::Hunter);

    REQUIRE(ccny.campusCode() == CampusCode::CCNY);
    REQUIRE(brooklyn.campusCode() == CampusCode::Brooklyn);
    REQUIRE(hunter.campusCode() == CampusCode::Hunter);

    // All sub-factories report the same campus.
    REQUIRE(ccny.students().campusCode() == CampusCode::CCNY);
    REQUIRE(ccny.faculty().campusCode() == CampusCode::CCNY);
    REQUIRE(ccny.teachingAssistants().campusCode() == CampusCode::CCNY);
    REQUIRE(ccny.admins().campusCode() == CampusCode::CCNY);
    REQUIRE(ccny.courses().campusCode() == CampusCode::CCNY);
    REQUIRE(ccny.sections().campusCode() == CampusCode::CCNY);

    // End-to-end: build a member + a course + a section all from one campus.
    auto student = ccny.students().admit(Person{Name{"A", "B"}}, Department::CS, 3.5);
    auto course = ccny.courses().createCourse({Department::CS, 200, 1, "Algorithms", 80});
    auto section =
        ccny.sections().createSection({course,
                                       1,
                                       {MeetingSlot{DaySet::monWedFri(), 10 * 60, 11 * 60}},
                                       EmployeeId{500'000'001},
                                       40,
                                       false});

    REQUIRE(student.hasRole(RoleType::Student));
    REQUIRE(course.id().str() == "CSC 20100");
    REQUIRE(section.sectionId() == "01");
}
