#pragma once

#include <cstdint>
#include <stdexcept>

namespace cuny_sms {

// Days of the week as a bitmask. Sun=1<<0, Mon=1<<1, ..., Sat=1<<6.
struct DaySet {
public:
    enum : std::uint8_t {
        Sun = 1U << 0,
        Mon = 1U << 1,
        Tue = 1U << 2,
        Wed = 1U << 3,
        Thu = 1U << 4,
        Fri = 1U << 5,
        Sat = 1U << 6,
    };

    DaySet() = default;
    explicit constexpr DaySet(std::uint8_t bits) noexcept : bits_(bits) {}

    [[nodiscard]] constexpr std::uint8_t bits() const noexcept { return bits_; }

    [[nodiscard]] constexpr bool contains(std::uint8_t day) const noexcept {
        return (bits_ & day) != 0;
    }

    [[nodiscard]] constexpr bool overlaps(DaySet other) const noexcept {
        return (bits_ & other.bits_) != 0;
    }

    // Common patterns.
    [[nodiscard]] static constexpr DaySet monWedFri() noexcept { return DaySet{Mon | Wed | Fri}; }
    [[nodiscard]] static constexpr DaySet tueThu() noexcept { return DaySet{Tue | Thu}; }
    [[nodiscard]] static constexpr DaySet monWed() noexcept { return DaySet{Mon | Wed}; }

    [[nodiscard]] friend constexpr bool operator==(DaySet a, DaySet b) noexcept = default;

private:
    std::uint8_t bits_{0};
};

// A single meeting time. Times are minutes since midnight; we don't model
// overnight slots (split into two slots if you really need that).
struct MeetingSlot {
public:
    MeetingSlot(DaySet days, std::int16_t startMinute, std::int16_t endMinute)
        : days_(days), start_(startMinute), end_(endMinute) {
        if (start_ < 0 || start_ >= 24 * 60) {
            throw std::invalid_argument("MeetingSlot: start out of range [0, 1440)");
        }
        if (end_ <= start_ || end_ > 24 * 60) {
            throw std::invalid_argument("MeetingSlot: end must be >start and <=1440");
        }
    }

    [[nodiscard]] constexpr DaySet days() const noexcept { return days_; }
    [[nodiscard]] constexpr std::int16_t start() const noexcept { return start_; }
    [[nodiscard]] constexpr std::int16_t end() const noexcept { return end_; }

    // Standard pairwise interval-overlap predicate, gated by day overlap.
    // (Algorithm chapter: docs/algorithms/08-schedule-conflict.md)
    [[nodiscard]] constexpr bool overlaps(const MeetingSlot& other) const noexcept {
        return days_.overlaps(other.days_) && start_ < other.end_ && other.start_ < end_;
    }

    [[nodiscard]] friend constexpr bool operator==(const MeetingSlot& a,
                                                   const MeetingSlot& b) noexcept = default;

private:
    DaySet days_;
    std::int16_t start_;
    std::int16_t end_;
};

}  // namespace cuny_sms
