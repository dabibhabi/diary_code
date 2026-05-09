# 08 — Schedule Conflict Detection

A student's enrolled set and a candidate course must not have overlapping meeting times. The hard part is "overlap" — the rest is bookkeeping.

## Time-slot model

A course has a list of `MeetingSlot`s. Each slot is `(daysOfWeek, startMinute, endMinute)`. A "conflict" is: any slot of the candidate overlaps any slot of any enrolled course.

```cpp
struct MeetingSlot {
    DaySet  days;          // bitmask of {Mon, Tue, Wed, Thu, Fri, Sat, Sun}
    int16_t startMinute;   // minutes since midnight
    int16_t endMinute;
};
```

Two slots overlap when:

$$\text{daysOverlap} \;\wedge\; \text{start}_a < \text{end}_b \;\wedge\; \text{start}_b < \text{end}_a$$

`daysOverlap` is `(slotA.days & slotB.days) != 0`. The time-overlap predicate is the standard "two intervals overlap iff each starts before the other ends".

## Algorithm choices by scale

| n (slots involved) | Algorithm | Complexity | Notes |
|--------------------|-----------|------------|-------|
| ≤ ~20 (one student) | Pairwise | O(n²) | Wins by constant factor; cache-hot |
| 50–500 (room availability) | Interval tree | O(log n) per query | Build once, query many |
| 10⁴+ (full catalog batch validation) | Sweep-line | O(n log n) | One pass detects all overlaps |

### Pairwise — the right answer for one student

A student typically has ≤ 6 enrolled courses, each with ≤ 3 meeting slots. That's ≤ 18 slots total. The candidate adds ≤ 3 more. 18 × 3 = 54 pair checks, each a handful of integer comparisons. **Don't reach for fancier here** — the constant factors of an interval tree dominate at n < ~50.

### Interval tree — for room-availability queries

A room hosts hundreds of slots per term. "Is room R204 free Tuesday 14:00–15:30?" is an interval-stabbing query. An interval tree (centered or augmented red-black tree) answers in O(log n + k) where k is the number of overlaps reported.

Build per room, refreshed when the schedule changes. The library choice in C++ is either a hand-rolled augmented `std::set` or `boost::icl::interval_set`.

### Sweep-line — for "validate the whole catalog"

When the registrar uploads next term's schedule, you want to detect all room conflicts at once. Sweep-line:

```text
events = [(slot.start, OPEN, slot), (slot.end, CLOSE, slot) for slot in slots]
sort events by (time, CLOSE before OPEN to handle adjacent intervals)
active = empty set
for (time, kind, slot) in events:
    if kind == OPEN:
        for s in active where s.room == slot.room:
            report conflict (slot, s)
        active.add(slot)
    else:
        active.remove(slot)
```

O(n log n) for the sort; the inner loop is O(k) per overlap. Output is the full conflict list — useful for an admin "fix the schedule before publishing" workflow.

A real implementation handles per-day separately (sweep within Monday, then Tuesday, etc.) since `daysOfWeek` is bitmasked; alternatively, expand each slot into one event per day before sweeping.

### Segment tree with lazy propagation — for room availability windows

If you want "how many rooms are free at 13:30 on Tuesdays?" or "mark this room busy 14:00–15:30 every Monday for the term", a segment tree with lazy propagation supports range-update + point-query in O(log n). Overkill for the first build; relevant if room assignment becomes a real subsystem.

## Strategy pattern wrapping these

The class diagram in `04-patterns.md` shows the Strategy structure. Each concrete strategy implements one of the algorithms above and returns a `ConflictResult`. `CompositeConflictStrategy` runs them in cheap-to-expensive order, short-circuiting on first conflict.

## Edge case: cross-day overlap

A class meeting Mon-Wed-Fri 23:30–00:30 doesn't really cross days (we forbid that), but be defensive: assert `endMinute > startMinute`. If we ever support overnight slots, split into `(Mon 23:30–24:00)` + `(Tue 00:00–00:30)` at the data layer rather than complicate the comparator.

## Edge case: TA assignments

A TA's lab section is a `TeachingAssistantRole.assignedCourses` entry, **not** an enrollment. The `RoleAwareStrategy` (see `04-patterns.md`) explicitly excludes TA assignments from conflict checks — TA hours are flexible work time, not student time. Campus policy could choose otherwise; that's a configuration knob, not a code change.

## Cross-references

- Strategy class diagram — `04-patterns.md`
- Bipartite matching for student → section assignment (different problem, related shape) — `math/17-linear-algebra-graphs.md`
- LP for capacity allocation under constraints — `math/16-optimization.md`
