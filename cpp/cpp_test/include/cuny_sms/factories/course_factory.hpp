#pragma once

#include <cstdint>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>

#include "cuny_sms/domain/campus_code.hpp"
#include "cuny_sms/domain/course.hpp"
#include "cuny_sms/domain/course_id.hpp"
#include "cuny_sms/domain/department.hpp"

namespace cuny_sms {

// Abstract factory for course catalog entries. Each campus has its own naming
// convention for the course id; the *meaning* (Department, level, name) is
// universal. Provide canonical inputs; receive the campus-local Course.
struct CourseSpec {
    Department department;
    Level level;                // 100, 200, 300, 400, 500
    std::int16_t sequence;      // 1, 2, 3 within a level (Calc I, II, III)
    std::string canonicalName;  // "Calculus I", "Intro to CS"
    std::int32_t capacity;
};

class CourseFactory {
public:
    CourseFactory(const CourseFactory&) = delete;
    CourseFactory& operator=(const CourseFactory&) = delete;
    CourseFactory(CourseFactory&&) = delete;
    CourseFactory& operator=(CourseFactory&&) = delete;
    virtual ~CourseFactory() = default;

    [[nodiscard]] virtual CampusCode campusCode() const noexcept = 0;

    // Produces a Course whose id follows the campus naming convention.
    [[nodiscard]] virtual Course createCourse(const CourseSpec& spec) const = 0;

    // Exposes the prefix mapping so tests and admin tooling can inspect it.
    [[nodiscard]] virtual std::string_view departmentPrefix(Department d) const noexcept = 0;

protected:
    CourseFactory() = default;

    static void validate(const CourseSpec& spec) {
        if (spec.level < 100 || spec.level > 599) {
            throw std::invalid_argument("CourseSpec: level must be in [100, 599]");
        }
        if (spec.sequence < 1 || spec.sequence > 99) {
            throw std::invalid_argument("CourseSpec: sequence must be in [1, 99]");
        }
        if (spec.canonicalName.empty()) {
            throw std::invalid_argument("CourseSpec: canonicalName must not be empty");
        }
        if (spec.capacity < 1) {
            throw std::invalid_argument("CourseSpec: capacity must be positive");
        }
    }
};

// ---------------------------------------------------------------------------
// CCNY style: "MATH 20100", "CSC 10200", "PHYS 20700"
//   prefix " " + level/100 + (sequence as 2-digit) + "00"
// ---------------------------------------------------------------------------
class CcnyCourseFactory final : public CourseFactory {
public:
    [[nodiscard]] CampusCode campusCode() const noexcept override { return CampusCode::CCNY; }

    [[nodiscard]] std::string_view departmentPrefix(Department d) const noexcept override {
        switch (d) {
        case Department::Math:
            return "MATH";
        case Department::CS:
            return "CSC";
        case Department::Stats:
            return "STAT";
        case Department::Physics:
            return "PHYS";
        }
        return "UNK";
    }

    [[nodiscard]] Course createCourse(const CourseSpec& spec) const override {
        validate(spec);
        std::ostringstream os;
        os << departmentPrefix(spec.department) << ' ' << (spec.level / 100)
           << (spec.sequence < 10 ? "0" : "") << spec.sequence << "00";
        return Course{CourseId{os.str()}, campusCode(),       spec.department,
                      spec.level,         spec.canonicalName, spec.capacity};
    }
};

// ---------------------------------------------------------------------------
// Brooklyn style: "MATH 1201", "CISC 1115", "PHYS 2100"
//   prefix " " + level/100 + (sequence as 3-digit zero-padded)
// ---------------------------------------------------------------------------
class BrooklynCourseFactory final : public CourseFactory {
public:
    [[nodiscard]] CampusCode campusCode() const noexcept override { return CampusCode::Brooklyn; }

    [[nodiscard]] std::string_view departmentPrefix(Department d) const noexcept override {
        switch (d) {
        case Department::Math:
            return "MATH";
        case Department::CS:
            return "CISC";
        case Department::Stats:
            return "STAT";
        case Department::Physics:
            return "PHYS";
        }
        return "UNK";
    }

    [[nodiscard]] Course createCourse(const CourseSpec& spec) const override {
        validate(spec);
        std::ostringstream os;
        os << departmentPrefix(spec.department) << ' ' << (spec.level / 100);
        if (spec.sequence < 10)
            os << "00";
        else if (spec.sequence < 100)
            os << "0";
        os << spec.sequence;
        return Course{CourseId{os.str()}, campusCode(),       spec.department,
                      spec.level,         spec.canonicalName, spec.capacity};
    }
};

// ---------------------------------------------------------------------------
// Hunter style: "MATH 201", "CSCI 127", "PHYSC 110"
//   prefix " " + level/100 + (sequence as 2-digit zero-padded)
// ---------------------------------------------------------------------------
class HunterCourseFactory final : public CourseFactory {
public:
    [[nodiscard]] CampusCode campusCode() const noexcept override { return CampusCode::Hunter; }

    [[nodiscard]] std::string_view departmentPrefix(Department d) const noexcept override {
        switch (d) {
        case Department::Math:
            return "MATH";
        case Department::CS:
            return "CSCI";
        case Department::Stats:
            return "STAT";
        case Department::Physics:
            return "PHYSC";
        }
        return "UNK";
    }

    [[nodiscard]] Course createCourse(const CourseSpec& spec) const override {
        validate(spec);
        std::ostringstream os;
        os << departmentPrefix(spec.department) << ' ' << (spec.level / 100)
           << (spec.sequence < 10 ? "0" : "") << spec.sequence;
        return Course{CourseId{os.str()}, campusCode(),       spec.department,
                      spec.level,         spec.canonicalName, spec.capacity};
    }
};

}  // namespace cuny_sms
