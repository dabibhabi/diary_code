#pragma once

#include <cstdint>
#include <string>
#include <string_view>
#include <utility>

#include "cuny_sms/domain/campus_code.hpp"
#include "cuny_sms/domain/course_id.hpp"
#include "cuny_sms/domain/department.hpp"

namespace cuny_sms {

// Course level ladder. 100=intro, 200=sophomore, 300=junior, 400=senior, 500=grad.
// Stored as an int for arithmetic ("the level above").
using Level = std::int16_t;

// A teachable unit, owned by a campus. The id is campus-specific (CCNY's
// "MATH 20100" vs Brooklyn's "MATH 1201"), but the *meaning* — Department +
// level + canonical name — is universal.
struct Course {
public:
    Course(CourseId id, CampusCode campus, Department department, Level level, std::string name,
           std::int32_t capacity)
        : id_(std::move(id)), campus_(campus), department_(department), level_(level),
          name_(std::move(name)), capacity_(capacity) {}

    [[nodiscard]] const CourseId& id() const noexcept { return id_; }
    [[nodiscard]] CampusCode campus() const noexcept { return campus_; }
    [[nodiscard]] Department department() const noexcept { return department_; }
    [[nodiscard]] Level level() const noexcept { return level_; }
    [[nodiscard]] std::string_view name() const noexcept { return name_; }
    [[nodiscard]] std::int32_t capacity() const noexcept { return capacity_; }

    [[nodiscard]] friend bool operator==(const Course& a, const Course& b) noexcept {
        return a.id_ == b.id_ && a.campus_ == b.campus_;
    }

private:
    CourseId id_;
    CampusCode campus_;
    Department department_;
    Level level_;
    std::string name_;
    std::int32_t capacity_;
};

}  // namespace cuny_sms
