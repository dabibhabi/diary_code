# 09 — Waitlist Queues

A waitlist is a queue of students waiting for a seat in a full course. The queue's data structure determines the policy: pure FIFO, priority-based, or priority-with-mutation.

## Phase 4 default: FIFO `std::deque`

The simplest correct thing. Push at back, pop at front, both O(1). Position numbering is implicit — index in the deque.

```cpp
class FifoWaitlist {
    std::deque<MemberId> queue_;
public:
    void enqueue(MemberId id) { queue_.push_back(id); }
    std::optional<MemberId> dequeue() {
        if (queue_.empty()) return std::nullopt;
        auto id = queue_.front();
        queue_.pop_front();
        return id;
    }
    [[nodiscard]] std::size_t size() const noexcept { return queue_.size(); }
    [[nodiscard]] std::optional<std::size_t> position(MemberId id) const {
        auto it = std::ranges::find(queue_, id);
        return (it == queue_.end()) ? std::nullopt
                                    : std::optional<std::size_t>{
                                        static_cast<std::size_t>(it - queue_.begin())
                                      };
    }
};
```

`position()` is O(n). For a 50-person waitlist that's nothing. For a 5000-person waitlist (CUNY freshman calculus, registration day), it's ~5 µs — still nothing. We'd revisit if a UI poll becomes a hot path, in which case a `std::unordered_map<MemberId, std::list::iterator>` lets us read position via index arithmetic on a `std::list` instead — but a list trades cache locality for that, so benchmark before believing it.

## Priority binary heap — `std::priority_queue`

Once policy says "seniors get promoted first" (or "students who already failed CS101 once get priority"), we move to a heap.

```cpp
struct WaitlistEntry {
    MemberId       id;
    int            priority;     // higher first
    std::int64_t   queuedAtNs;   // tie-breaker: FIFO within priority
    auto operator<=>(const WaitlistEntry& other) const {
        if (auto c = priority <=> other.priority; c != 0) return c;
        return other.queuedAtNs <=> queuedAtNs;  // earlier wins
    }
};
```

Push: O(log n). Pop max: O(log n). Position-of-id: not directly supported by `std::priority_queue` — fall back to a parallel `unordered_map<MemberId, EntryRef>` if needed.

Trade-off: heaps don't maintain a stable position number. "You're #4 in the queue" stops being meaningful when priority varies. UI must communicate "estimated position" instead of a hard number.

## Fibonacci heap — when priorities mutate

If a senior's status is granted mid-term and we want to *promote* their existing waitlist entry without re-queueing, we need `decrease-key` (or `increase-key`). Operations:

| Operation | Binary heap | Fibonacci heap |
|-----------|-------------|----------------|
| insert | O(log n) | O(1) am. |
| extract-max | O(log n) | O(log n) am. |
| decrease-key (or increase, for max-heap) | O(log n) | **O(1) am.** |
| meld (combine two heaps) | O(n) | O(1) am. |

Fibonacci heap's amortized O(1) for `decrease-key` is the famous one. In practice the constants are large enough that for n < ~10⁴ a pairing heap or even a binary heap with the entry rebuilt on priority change is faster.

## Pairing heap — practical alternative

Pairing heaps are simpler to implement, faster in practice than Fibonacci heaps, and have very nearly the same amortized bounds (O(log n) extract; O(1) am. decrease-key conjectured, proved O(log n)). If we ever need decrease-key in production, this is the choice.

## Renumber-on-pop vs lazy positions

When Carol (#1) gets promoted, what happens to Dave (#2) and Eve (#3)?

| Strategy | Pros | Cons |
|----------|------|------|
| **Renumber on pop** (Dave 2→1, Eve 3→2) | Position number is always meaningful and stable | Every promotion is O(n) writes to renumber |
| **Lazy positions** (Dave stays #2, Eve stays #3 until next pop) | O(1) promotion | Position numbers eventually have gaps; UI must sort by position |

The current design (`05-flows.md`) renumbers on pop because a 50-person typical queue makes the O(n) cost trivial and the UX is much cleaner. For 5000-person queues, lazy positions win.

## Atomicity

Promotion = `(remove from waitlist) + (insert enrollment) + (renumber)`. All three must commit together; otherwise a crash leaves Carol both waitlisted and enrolled, or worse, removed from the waitlist with no enrollment. The transaction boundary is `WaitlistManager::promoteFromWaitlist` (see `05-flows.md`); for the in-memory implementation in Phase 6, a single mutex guarding the queue + enrollment vector suffices. Once SQLite arrives (Phase 7+), it's a real SQL transaction.

## Probability hooks

Two natural probabilistic questions about a waitlist:

- **Expected wait time** — model arrivals as Poisson with rate $$\lambda$$ and "drops opening seats" as exponential with rate $$\mu$$. M/M/1 expected wait is $$W = \frac{1}{\mu - \lambda}$$. See `math/13-probability.md`.
- **Probability of promotion before term start** — if drops follow a Poisson process and the registration window is `T` days, the probability of at least `k` drops is $$P(N \ge k) = 1 - \sum_{j=0}^{k-1} \frac{(\lambda T)^j e^{-\lambda T}}{j!}$$.

Both are good demo notebooks for the math chapters.

## Cross-references

- Drop-and-promote flow — `05-flows.md`
- M/M/1 derivation — `math/13-probability.md`
- Min-cost flow alternative for student → section — `math/16-optimization.md`
