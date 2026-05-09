# 11 — Caching & Memoization

The two memoizable computations in this codebase: **prereq closures** (used on every enrollment eligibility check) and **corequisite groups** (rare lookup, but the data structure underlying it — Union-Find — is reused elsewhere).

## LRU cache for prereq closures

`PrerequisiteGraph::transitiveClosure(courseId)` returns a `std::vector<CourseId>`. We compute it on demand and cache it; invalidate on `addPrereq`/`removePrereq`.

### The classic implementation

`std::list` (doubly linked, for O(1) splice-to-front) + `std::unordered_map<Key, list::iterator>` (for O(1) lookup).

```cpp
template <typename Key, typename Value>
class LruCache {
    using ListIt = typename std::list<std::pair<Key, Value>>::iterator;
    std::size_t capacity_;
    std::list<std::pair<Key, Value>> order_;     // front = most recent
    std::unordered_map<Key, ListIt> index_;
public:
    explicit LruCache(std::size_t cap) : capacity_(cap) {}

    [[nodiscard]] std::optional<std::reference_wrapper<const Value>>
    get(const Key& k) {
        auto it = index_.find(k);
        if (it == index_.end()) return std::nullopt;
        order_.splice(order_.begin(), order_, it->second);  // touch
        return std::cref(it->second->second);
    }

    void put(Key k, Value v) {
        if (auto it = index_.find(k); it != index_.end()) {
            it->second->second = std::move(v);
            order_.splice(order_.begin(), order_, it->second);
            return;
        }
        if (order_.size() == capacity_) {
            index_.erase(order_.back().first);
            order_.pop_back();
        }
        order_.emplace_front(k, std::move(v));
        index_[std::move(k)] = order_.begin();
    }

    void invalidate(const Key& k) {
        if (auto it = index_.find(k); it != index_.end()) {
            order_.erase(it->second);
            index_.erase(it);
        }
    }
};
```

`get` is O(1) average; `put` is O(1) average; `invalidate` is O(1) average. Capacity is a knob; for the prereq graph, "every course in the catalog" is a few hundred entries — pin all of them in cache and skip eviction entirely.

### Invalidation on graph mutation

When `addPrereq(courseId, prereqId)` runs, every *descendant* of `courseId` has a stale closure. Either:

1. **Eager**: walk descendants, invalidate each. Cost: O(D) where D is the descendant count. Simple.
2. **Lazy versioning**: bump a global graph version; cached closures store the version they were computed under; lookup compares versions and recomputes on mismatch. Cost: O(1) on edit, O(1) on hit. Slightly more code.

For a catalog of ~500 courses, eager wins on simplicity.

## Union-Find for corequisite groups

Corequisites are courses that must be taken together (lecture + lab; CS systems lecture + lab section). They form an equivalence relation: if A requires B and B requires C, then {A, B, C} is a corequisite cluster.

Union-Find (a.k.a. disjoint-set union) supports two operations on a partition:

- `find(x)` — returns the representative of x's set
- `union(x, y)` — merges the sets containing x and y

With **path compression** (during `find`, point every node directly at the root) and **union by rank** (in `union`, attach the shallower tree under the deeper), both operations run in nearly-constant amortized time:

$$O(\alpha(n))$$

where $$\alpha$$ is the inverse Ackermann function — bounded by 4 for any conceivable n.

```cpp
class UnionFind {
    std::vector<std::size_t> parent_;
    std::vector<std::size_t> rank_;
public:
    explicit UnionFind(std::size_t n)
        : parent_(n), rank_(n, 0) {
        std::iota(parent_.begin(), parent_.end(), std::size_t{0});
    }
    std::size_t find(std::size_t x) {
        while (parent_[x] != x) {
            parent_[x] = parent_[parent_[x]];   // path halving
            x = parent_[x];
        }
        return x;
    }
    bool unite(std::size_t a, std::size_t b) {
        a = find(a); b = find(b);
        if (a == b) return false;
        if (rank_[a] < rank_[b]) std::swap(a, b);
        parent_[b] = a;
        if (rank_[a] == rank_[b]) ++rank_[a];
        return true;
    }
};
```

Use case beyond corequisites: connected-component analysis on the prereq DAG (treated undirected), Kruskal's MST if we ever weight prereqs by recommended order strength, equivalence classes of "interchangeable" elective slots.

## When to reach for what

| Need | Tool |
|------|------|
| "Is this expensive computation cached?" | LRU |
| "Are these two things in the same group?" | Union-Find |
| "What's the closure of X under transitive relation R?" | LRU + DFS |
| "How many groups are there?" | Union-Find with size tracking |
| "What's the largest group?" | Union-Find with size tracking |

## Forward-looking: skip lists, B+-trees

Once SQLite arrives (Phase 7+), on-disk indexes become relevant. We don't implement them — SQLite does — but it's worth knowing why they exist:

- **B+-tree** (SQLite's choice): all data in leaves, internal nodes are routing only. High fan-out keeps depth ~3–4 even for billions of rows. Sequential scans walk the leaf chain.
- **Skip list** (Redis sorted sets, LevelDB): probabilistic balance, O(log n) expected operations, simpler to implement than balanced BSTs, lock-free variants exist.

Neither belongs in the in-memory phase, but the existence of cache-vs-disk hierarchy informs the dual-index design: in-memory hash maps for hot data, B+-trees on disk for cold data, eventually a third tier (SSD-backed perfect hash, see `07-attendance-indexing.md`) for frozen historical terms.

## Cross-references

- Where the closure cache lives architecturally — `02-architecture.md`, flowchart in `06-prerequisite-graph.md`
- Cache-line layout for hash buckets — `07-attendance-indexing.md`
- Combinatorial size of equivalence classes — `math/15-combinatorics.md`
