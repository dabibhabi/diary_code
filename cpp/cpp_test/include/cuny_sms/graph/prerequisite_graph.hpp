#pragma once

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <list>
#include <optional>
#include <queue>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "cuny_sms/domain/course_id.hpp"

// CourseId hash — the graph's adjacency map keys on CourseId, so we provide a
// std::hash specialization here. Lives in namespace std per the standard.
namespace std {

template <>
struct hash<::cuny_sms::CourseId> {
    [[nodiscard]] std::size_t operator()(const ::cuny_sms::CourseId& id) const noexcept {
        return std::hash<std::string>{}(id.str());
    }
};

}  // namespace std

namespace cuny_sms {

// Thrown by PrerequisiteGraph when a mutation would create a cycle, or when
// Kahn's topo sort detects an unexpected cycle (defense in depth).
class PrereqCycleError : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};

// PrerequisiteGraph — the directed acyclic graph of "X must be taken before Y".
//
// Edges point from prerequisite -> dependent course (so "CS101 must come before
// CS102" is the edge CS101 -> CS102). The graph is kept acyclic by running a
// 3-color DFS on every addPrereq; any cycle-introducing edge is rejected with
// PrereqCycleError before mutation is committed.
//
// AND/OR semantics:
//   - PrereqEdge::groupKey == ""    -> AND with everything (must complete this).
//   - PrereqEdge::groupKey == "G"   -> OR within the group (any one of "G" suffices).
//   - Distinct groupKeys plus the ungrouped bucket combine with AND.
//
// Algorithms (see docs/algorithms/06-prerequisite-graph.md):
//   - addPrereq cycle check         : 3-color DFS, O(V + E)
//   - findCycles                    : Tarjan's SCC, O(V + E)
//   - topologicalOrder              : Kahn's BFS, O(V + E)
//   - transitiveClosure             : DFS with LRU memoization
//   - shortestPath                  : BFS layering, O(V + E)
//   - isSatisfied                   : AND across distinct group keys, OR within
//
// Cache invalidation: a mutation invalidates the closure cache for the affected
// course AND every descendant of that course (eager strategy per
// docs/algorithms/11-caching-memoization.md).
class PrerequisiteGraph {
public:
    // One directed prerequisite relation: `prerequisite` must come before
    // `course`. `groupKey` empty means a strict AND; same-string keys form an
    // OR alternative group within the target's prereq set.
    struct PrereqEdge {
        CourseId course;
        CourseId prerequisite;
        std::string groupKey;
    };

    PrerequisiteGraph() = default;

    // Mutators ---------------------------------------------------------------

    // Insert a course node with no edges. Idempotent.
    void addCourse(CourseId id) {
        if (forward_.find(id) == forward_.end()) {
            invalidateClosureFor(id);
            forward_.emplace(id, std::vector<PrereqEdge>{});
            reverse_.emplace(id, std::vector<CourseId>{});
        }
    }

    // Add a prerequisite edge; throws PrereqCycleError if it would create a
    // cycle (including a self-loop). Idempotent on exact duplicates.
    // groupKey is taken by value because it is moved into the new PrereqEdge.
    void addPrereq(  // NOLINT(performance-unnecessary-value-param)
        CourseId course, CourseId prerequisite, std::string groupKey = "") {
        if (course == prerequisite) {
            throw PrereqCycleError("PrereqCycleError: self-loop on '" + course.str() + "'");
        }

        addCourse(course);
        addCourse(prerequisite);

        // Skip exact duplicates (same edge + same group).
        auto& edges = forward_[course];
        for (const auto& e : edges) {
            if (e.prerequisite == prerequisite && e.groupKey == groupKey) {
                return;
            }
        }

        // Speculative cycle check before commit. The new edge is
        // `prerequisite` -> `course` (in the prereq-to-dependent direction),
        // so a cycle exists iff `course` already (transitively) reaches
        // `prerequisite` along existing prereq->dependent edges.
        if (reaches(course, prerequisite)) {
            throw PrereqCycleError("PrereqCycleError: edge '" + prerequisite.str() + "' -> '" +
                                   course.str() + "' would create a cycle");
        }

        edges.push_back(PrereqEdge{course, prerequisite, std::move(groupKey)});
        reverse_[prerequisite].push_back(course);

        invalidateClosureFor(course);
    }

    // Remove an edge. Returns true if removed, false if no such edge existed.
    bool removePrereq(const CourseId& course, const CourseId& prerequisite) {
        auto it = forward_.find(course);
        if (it == forward_.end()) {
            return false;
        }
        auto& edges = it->second;
        const auto before = edges.size();
        edges.erase(
            std::remove_if(edges.begin(), edges.end(),
                           [&](const PrereqEdge& e) { return e.prerequisite == prerequisite; }),
            edges.end());
        if (edges.size() == before) {
            return false;
        }

        // Mirror the removal in the reverse adjacency.
        auto rIt = reverse_.find(prerequisite);
        if (rIt != reverse_.end()) {
            auto& rev = rIt->second;
            rev.erase(std::remove(rev.begin(), rev.end(), course), rev.end());
        }

        invalidateClosureFor(course);
        return true;
    }

    // Queries ----------------------------------------------------------------

    [[nodiscard]] std::size_t courseCount() const noexcept { return forward_.size(); }

    // All transitive prerequisites of `course` (does NOT include `course`
    // itself). Memoized; later calls with the same key are O(1) lookups until
    // the cache entry is invalidated by a mutation.
    [[nodiscard]] std::vector<CourseId> transitiveClosure(const CourseId& course) const {
        if (auto cached = closureCache_.get(course)) {
            return cached->get();
        }

        std::vector<CourseId> result;
        std::unordered_set<CourseId> seen;
        // Iterative DFS — protects the stack on pathological depths.
        std::vector<CourseId> stack;
        stack.push_back(course);

        while (!stack.empty()) {
            CourseId current = std::move(stack.back());
            stack.pop_back();

            auto it = forward_.find(current);
            if (it == forward_.end()) {
                continue;
            }
            for (const auto& edge : it->second) {
                if (seen.insert(edge.prerequisite).second) {
                    result.push_back(edge.prerequisite);
                    stack.push_back(edge.prerequisite);
                }
            }
        }

        closureCache_.put(course, result);
        return result;
    }

    // Kahn's algorithm: produce a topological order of all known courses.
    // Throws PrereqCycleError if the graph contains a cycle (defense in depth —
    // addPrereq should already have prevented this).
    //
    // In our adjacency representation, indegree(X) — counting incoming
    // prereq->dependent edges — equals the number of prerequisites X has,
    // i.e., forward_[X].size().
    [[nodiscard]] std::vector<CourseId> topologicalOrder() const {
        std::unordered_map<CourseId, std::size_t> indegree;
        indegree.reserve(forward_.size());
        for (const auto& [node, edges] : forward_) {
            indegree[node] = edges.size();
        }

        std::queue<CourseId> queue;
        for (const auto& [node, deg] : indegree) {
            if (deg == 0) {
                queue.push(node);
            }
        }

        std::vector<CourseId> order;
        order.reserve(forward_.size());
        while (!queue.empty()) {
            CourseId v = std::move(queue.front());
            queue.pop();
            order.push_back(v);

            // Discharge each dependent's indegree by 1.
            auto it = reverse_.find(v);
            if (it == reverse_.end()) {
                continue;
            }
            for (const CourseId& dependent : it->second) {
                auto& deg = indegree[dependent];
                --deg;
                if (deg == 0) {
                    queue.push(dependent);
                }
            }
        }

        if (order.size() != forward_.size()) {
            throw PrereqCycleError(
                "PrereqCycleError: topologicalOrder failed — graph contains a cycle");
        }
        return order;
    }

    // Tarjan's SCC. Returns every strongly-connected component of size >= 2
    // (i.e., every cycle / set of intersecting cycles). Singletons are
    // omitted because in a DAG every node is its own trivial SCC.
    [[nodiscard]] std::vector<std::vector<CourseId>> findCycles() const {
        TarjanState state;
        state.indexOf.reserve(forward_.size());
        state.lowlink.reserve(forward_.size());
        state.onStack.reserve(forward_.size());

        for (const auto& [node, _] : forward_) {
            if (state.indexOf.find(node) == state.indexOf.end()) {
                tarjanStrongConnect(node, state);
            }
        }
        return state.result;
    }

    // BFS in the prereq direction (from -> to). Returns the path including
    // both endpoints when reachable, or empty vector when unreachable. The
    // traversal follows edges prerequisite -> dependent; "from" must be a
    // prerequisite of (or equal to) "to" for a non-empty answer.
    [[nodiscard]] std::vector<CourseId> shortestPath(const CourseId& from,
                                                     const CourseId& to) const {
        if (forward_.find(from) == forward_.end() || forward_.find(to) == forward_.end()) {
            return {};
        }
        if (from == to) {
            return {from};
        }

        std::unordered_map<CourseId, CourseId> parent;
        std::unordered_set<CourseId> visited;
        std::queue<CourseId> queue;
        queue.push(from);
        visited.insert(from);

        while (!queue.empty()) {
            CourseId v = std::move(queue.front());
            queue.pop();

            auto it = reverse_.find(v);
            if (it == reverse_.end()) {
                continue;
            }
            for (const CourseId& dependent : it->second) {
                if (visited.insert(dependent).second) {
                    parent.emplace(dependent, v);
                    if (dependent == to) {
                        return reconstructPath(from, to, parent);
                    }
                    queue.push(dependent);
                }
            }
        }
        return {};
    }

    // AND/OR boolean evaluation: true if `completed` covers every prereq group
    // of `target`. For each distinct non-empty groupKey, OR over members; for
    // ungrouped edges, every one is required.
    [[nodiscard]] bool isSatisfied(const CourseId& target,
                                   const std::unordered_set<CourseId>& completed) const {
        auto it = forward_.find(target);
        if (it == forward_.end()) {
            return true;  // unknown target has no recorded prereqs
        }

        // Bucket prereqs by groupKey. Ungrouped edges (empty key) all live in
        // the implicit "" bucket and combine via AND.
        std::unordered_map<std::string, std::vector<CourseId>> groups;
        for (const auto& edge : it->second) {
            if (edge.course == target) {
                groups[edge.groupKey].push_back(edge.prerequisite);
            }
        }

        // Iterate the forward edges (recall: forward_[X] holds edges whose
        // .course == X), so we reach here knowing every edge belongs to
        // `target`. Note: the precondition `edge.course == target` above is a
        // guard for any future refactor; today it always holds.
        for (const auto& [key, prereqs] : groups) {
            if (key.empty()) {
                // Ungrouped: every prereq is required (AND).
                for (const auto& p : prereqs) {
                    if (completed.find(p) == completed.end()) {
                        return false;
                    }
                }
            } else {
                // Grouped: at least one member of the group satisfies it (OR).
                const bool anyMet =
                    std::any_of(prereqs.begin(), prereqs.end(), [&](const CourseId& p) {
                        return completed.find(p) != completed.end();
                    });
                if (!anyMet) {
                    return false;
                }
            }
        }
        return true;
    }

private:
    static constexpr std::size_t kClosureCacheCapacity = 256;

    // Tiny LRU cache (capacity-bounded) — std::list for O(1) splice + a hash
    // map of Key -> list iterator. See docs/algorithms/11-caching-memoization.md.
    template <typename Key, typename Value>
    class LruCache {
    public:
        using Pair = std::pair<Key, Value>;
        using ListIt = typename std::list<Pair>::iterator;

        explicit LruCache(std::size_t cap) : capacity_(cap) {}

        [[nodiscard]] std::optional<std::reference_wrapper<const Value>> get(const Key& k) const {
            auto it = index_.find(k);
            if (it == index_.end()) {
                return std::nullopt;
            }
            order_.splice(order_.begin(), order_, it->second);
            return std::cref(it->second->second);
        }

        void put(const Key& k, Value v) const {
            if (auto it = index_.find(k); it != index_.end()) {
                it->second->second = std::move(v);
                order_.splice(order_.begin(), order_, it->second);
                return;
            }
            if (order_.size() >= capacity_) {
                index_.erase(order_.back().first);
                order_.pop_back();
            }
            order_.emplace_front(k, std::move(v));
            index_.emplace(k, order_.begin());
        }

        void invalidate(const Key& k) const {
            if (auto it = index_.find(k); it != index_.end()) {
                order_.erase(it->second);
                index_.erase(it);
            }
        }

        void clear() const {
            order_.clear();
            index_.clear();
        }

    private:
        std::size_t capacity_{0};
        mutable std::list<Pair> order_{};
        mutable std::unordered_map<Key, ListIt> index_{};
    };

    // Reachability test used by addPrereq's cycle check: is `target` reachable
    // from `start` along edges prerequisite -> dependent?
    [[nodiscard]] bool reaches(const CourseId& start, const CourseId& target) const {
        if (start == target) {
            return true;
        }
        std::unordered_set<CourseId> visited;
        std::vector<CourseId> stack;
        stack.push_back(start);
        visited.insert(start);

        while (!stack.empty()) {
            CourseId v = std::move(stack.back());
            stack.pop_back();

            auto it = reverse_.find(v);
            if (it == reverse_.end()) {
                continue;
            }
            for (const CourseId& dependent : it->second) {
                if (dependent == target) {
                    return true;
                }
                if (visited.insert(dependent).second) {
                    stack.push_back(dependent);
                }
            }
        }
        return false;
    }

    // Tarjan's algorithm bookkeeping in a single struct so the recursive
    // helper stays readable.
    struct TarjanState {
        std::unordered_map<CourseId, std::size_t> indexOf;
        std::unordered_map<CourseId, std::size_t> lowlink;
        std::unordered_set<CourseId> onStack;
        std::vector<CourseId> stack;
        std::vector<std::vector<CourseId>> result;
        std::size_t index{0};
    };

    void tarjanStrongConnect(const CourseId& v, TarjanState& s) const {
        s.indexOf[v] = s.index;
        s.lowlink[v] = s.index;
        ++s.index;
        s.stack.push_back(v);
        s.onStack.insert(v);

        // Walk forward edges in graph terms (prereq -> dependent), which our
        // representation stores in reverse_[v] (the dependents of v).
        auto it = reverse_.find(v);
        if (it != reverse_.end()) {
            for (const CourseId& w : it->second) {
                if (s.indexOf.find(w) == s.indexOf.end()) {
                    tarjanStrongConnect(w, s);
                    s.lowlink[v] = std::min(s.lowlink[v], s.lowlink[w]);
                } else if (s.onStack.find(w) != s.onStack.end()) {
                    s.lowlink[v] = std::min(s.lowlink[v], s.indexOf[w]);
                }
            }
        }

        if (s.lowlink[v] == s.indexOf[v]) {
            std::vector<CourseId> component;
            while (true) {
                CourseId w = std::move(s.stack.back());
                s.stack.pop_back();
                s.onStack.erase(w);
                component.push_back(w);
                if (w == v) {
                    break;
                }
            }
            if (component.size() >= 2) {
                s.result.push_back(std::move(component));
            }
        }
    }

    [[nodiscard]] static std::vector<CourseId>
    reconstructPath(const CourseId& from, const CourseId& to,
                    const std::unordered_map<CourseId, CourseId>& parent) {
        std::vector<CourseId> path;
        CourseId cur = to;
        path.push_back(cur);
        while (!(cur == from)) {
            auto it = parent.find(cur);
            if (it == parent.end()) {
                return {};  // shouldn't happen if to was discovered
            }
            cur = it->second;
            path.push_back(cur);
        }
        std::reverse(path.begin(), path.end());
        return path;
    }

    // Invalidate the closure-cache entry for `course` and every transitive
    // descendant (anything that lists `course` directly or indirectly as a
    // prerequisite).
    void invalidateClosureFor(const CourseId& course) {
        std::unordered_set<CourseId> visited;
        std::vector<CourseId> stack;
        stack.push_back(course);
        visited.insert(course);

        while (!stack.empty()) {
            CourseId v = std::move(stack.back());
            stack.pop_back();
            closureCache_.invalidate(v);

            auto it = reverse_.find(v);
            if (it == reverse_.end()) {
                continue;
            }
            for (const CourseId& dependent : it->second) {
                if (visited.insert(dependent).second) {
                    stack.push_back(dependent);
                }
            }
        }
    }

    // forward_[X] holds the edges whose .course == X — i.e., the prerequisites
    // of X. reverse_[X] is the inverse map: for each prereq X, the courses
    // that depend on X. Both are kept in sync by addPrereq / removePrereq.
    std::unordered_map<CourseId, std::vector<PrereqEdge>> forward_;
    std::unordered_map<CourseId, std::vector<CourseId>> reverse_;
    LruCache<CourseId, std::vector<CourseId>> closureCache_{kClosureCacheCapacity};
};

}  // namespace cuny_sms
