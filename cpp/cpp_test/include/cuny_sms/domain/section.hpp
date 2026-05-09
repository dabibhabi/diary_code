#pragma once

#include <cstdint>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "cuny_sms/domain/course_id.hpp"
#include "cuny_sms/domain/employee_id.hpp"
#include "cuny_sms/domain/meeting_slot.hpp"

namespace cuny_sms {

// One concrete instance of a course. CS101 the Course is offered as
// (CS101, Section 01, MWF 10-11, Room 204, Prof Smith) — that's a Section.
//
// Multiple sections of the same course can run in the same term, each with
// its own slots and professor. Conflict detection (Phase 4) operates on
// Section, not Course.
struct Section {
public:
    Section(CourseId courseId, std::string sectionId, std::vector<MeetingSlot> slots,
            EmployeeId professorId, std::int32_t capacity)
        : courseId_(std::move(courseId)), sectionId_(std::move(sectionId)),
          slots_(std::move(slots)), professorId_(professorId), capacity_(capacity) {}

    [[nodiscard]] const CourseId& courseId() const noexcept { return courseId_; }
    [[nodiscard]] std::string_view sectionId() const noexcept { return sectionId_; }
    [[nodiscard]] const std::vector<MeetingSlot>& slots() const noexcept { return slots_; }
    [[nodiscard]] EmployeeId professorId() const noexcept { return professorId_; }
    [[nodiscard]] std::int32_t capacity() const noexcept { return capacity_; }

    // Convenience: does *any* slot of this section conflict with any slot of `other`?
    [[nodiscard]] bool conflictsWith(const Section& other) const noexcept {
        for (const auto& a : slots_) {
            for (const auto& b : other.slots_) {
                if (a.overlaps(b))
                    return true;
            }
        }
        return false;
    }

private:
    CourseId courseId_;
    std::string sectionId_;
    std::vector<MeetingSlot> slots_;
    EmployeeId professorId_;
    std::int32_t capacity_;
};

}  // namespace cuny_sms
