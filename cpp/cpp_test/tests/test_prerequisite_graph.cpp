#include <catch2/catch_test_macros.hpp>

#include <algorithm>
#include <unordered_set>
#include <vector>

#include "cuny_sms/domain/course_id.hpp"
#include "cuny_sms/graph/prerequisite_graph.hpp"

using namespace cuny_sms;  // NOLINT(google-build-using-namespace) — test scope only

namespace {

// True iff `prereq` precedes `course` in `order` (both must be present).
[[nodiscard]] bool precedes(const std::vector<CourseId>& order, const CourseId& prereq,
                            const CourseId& course) {
    auto pIt = std::find(order.begin(), order.end(), prereq);
    auto cIt = std::find(order.begin(), order.end(), course);
    return pIt != order.end() && cIt != order.end() && pIt < cIt;
}

[[nodiscard]] bool contains(const std::vector<CourseId>& v, const CourseId& id) {
    return std::find(v.begin(), v.end(), id) != v.end();
}

// Canonical curriculum from docs/algorithms/06-prerequisite-graph.md. AD401
// requires CS301 AND (Math301 OR Math302).
[[nodiscard]] PrerequisiteGraph buildCanonicalCurriculum() {
    PrerequisiteGraph g;
    g.addPrereq(CourseId{"CS102"}, CourseId{"CS101"});
    g.addPrereq(CourseId{"CS201"}, CourseId{"CS102"});
    g.addPrereq(CourseId{"CS201"}, CourseId{"Math101"});
    g.addPrereq(CourseId{"CS301"}, CourseId{"CS102"});
    g.addPrereq(CourseId{"CS301"}, CourseId{"CS201"});
    g.addPrereq(CourseId{"AI301"}, CourseId{"CS201"});
    g.addPrereq(CourseId{"AI301"}, CourseId{"Math201"});
    g.addPrereq(CourseId{"Stats301"}, CourseId{"Math201"});
    g.addPrereq(CourseId{"Phys301"}, CourseId{"Math301"});
    g.addPrereq(CourseId{"Math201"}, CourseId{"Math101"});
    // AD401: CS301 AND (Math301 OR Math302)
    g.addPrereq(CourseId{"AD401"}, CourseId{"CS301"});
    g.addPrereq(CourseId{"AD401"}, CourseId{"Math301"}, "math-foundation");
    g.addPrereq(CourseId{"AD401"}, CourseId{"Math302"}, "math-foundation");
    return g;
}

}  // namespace

TEST_CASE("Empty graph reports zero courses and an empty topo order", "[prereq][empty]") {
    PrerequisiteGraph g;
    REQUIRE(g.courseCount() == 0);
    REQUIRE(g.topologicalOrder().empty());
    REQUIRE(g.findCycles().empty());
}

TEST_CASE("Single edge: addPrereq registers both courses and closure shows the prereq",
          "[prereq][closure]") {
    PrerequisiteGraph g;
    g.addPrereq(CourseId{"CS102"}, CourseId{"CS101"});
    REQUIRE(g.courseCount() == 2);
    auto closure = g.transitiveClosure(CourseId{"CS102"});
    REQUIRE(closure.size() == 1);
    REQUIRE(closure.front() == CourseId{"CS101"});

    // CS101 itself has no prerequisites.
    REQUIRE(g.transitiveClosure(CourseId{"CS101"}).empty());
}

TEST_CASE("addPrereq rejects a self-loop", "[prereq][cycle]") {
    PrerequisiteGraph g;
    REQUIRE_THROWS_AS(g.addPrereq(CourseId{"CS101"}, CourseId{"CS101"}), PrereqCycleError);
}

TEST_CASE("addPrereq rejects a 2-cycle A->B->A", "[prereq][cycle]") {
    PrerequisiteGraph g;
    g.addPrereq(CourseId{"B"}, CourseId{"A"});
    REQUIRE_THROWS_AS(g.addPrereq(CourseId{"A"}, CourseId{"B"}), PrereqCycleError);
}

TEST_CASE("addPrereq rejects an indirect cycle A->B->C->A", "[prereq][cycle]") {
    PrerequisiteGraph g;
    g.addPrereq(CourseId{"B"}, CourseId{"A"});
    g.addPrereq(CourseId{"C"}, CourseId{"B"});
    REQUIRE_THROWS_AS(g.addPrereq(CourseId{"A"}, CourseId{"C"}), PrereqCycleError);
}

TEST_CASE("findCycles returns empty on a valid DAG", "[prereq][tarjan]") {
    auto g = buildCanonicalCurriculum();
    REQUIRE(g.findCycles().empty());
}

TEST_CASE("topologicalOrder respects every edge of the canonical curriculum", "[prereq][topo]") {
    auto g = buildCanonicalCurriculum();
    auto order = g.topologicalOrder();
    REQUIRE(order.size() == g.courseCount());

    // Spot-check the canonical edge set.
    REQUIRE(precedes(order, CourseId{"CS101"}, CourseId{"CS102"}));
    REQUIRE(precedes(order, CourseId{"CS102"}, CourseId{"CS201"}));
    REQUIRE(precedes(order, CourseId{"CS201"}, CourseId{"CS301"}));
    REQUIRE(precedes(order, CourseId{"CS301"}, CourseId{"AD401"}));
    REQUIRE(precedes(order, CourseId{"Math101"}, CourseId{"Math201"}));
    REQUIRE(precedes(order, CourseId{"Math201"}, CourseId{"AI301"}));
    REQUIRE(precedes(order, CourseId{"Math201"}, CourseId{"Stats301"}));
    REQUIRE(precedes(order, CourseId{"Math301"}, CourseId{"Phys301"}));
    REQUIRE(precedes(order, CourseId{"Math301"}, CourseId{"AD401"}));
    REQUIRE(precedes(order, CourseId{"Math302"}, CourseId{"AD401"}));
}

TEST_CASE("transitiveClosure returns every transitive prereq of AD401",
          "[prereq][closure][canonical]") {
    auto g = buildCanonicalCurriculum();
    auto closure = g.transitiveClosure(CourseId{"AD401"});
    // Expected ancestry: CS301, CS201, CS102, CS101, Math101, Math301, Math302.
    REQUIRE(closure.size() == 7);
    REQUIRE(contains(closure, CourseId{"CS301"}));
    REQUIRE(contains(closure, CourseId{"CS201"}));
    REQUIRE(contains(closure, CourseId{"CS102"}));
    REQUIRE(contains(closure, CourseId{"CS101"}));
    REQUIRE(contains(closure, CourseId{"Math101"}));
    REQUIRE(contains(closure, CourseId{"Math301"}));
    REQUIRE(contains(closure, CourseId{"Math302"}));
}

TEST_CASE("transitiveClosure cache returns identical results on repeat queries",
          "[prereq][closure][cache]") {
    auto g = buildCanonicalCurriculum();
    auto first = g.transitiveClosure(CourseId{"AD401"});
    auto second = g.transitiveClosure(CourseId{"AD401"});
    auto third = g.transitiveClosure(CourseId{"AD401"});
    REQUIRE(first.size() == second.size());
    REQUIRE(second.size() == third.size());
    for (const auto& id : first) {
        REQUIRE(contains(second, id));
        REQUIRE(contains(third, id));
    }
}

TEST_CASE("isSatisfied: AND across groups, OR within a group", "[prereq][bool]") {
    auto g = buildCanonicalCurriculum();
    const CourseId target{"AD401"};

    // Missing every prereq.
    REQUIRE_FALSE(g.isSatisfied(target, {}));

    // CS301 (AND) satisfied + Math301 satisfies the OR group.
    {
        std::unordered_set<CourseId> done;
        done.insert(CourseId{"CS301"});
        done.insert(CourseId{"Math301"});
        REQUIRE(g.isSatisfied(target, done));
    }

    // CS301 satisfied + Math302 alone for the OR group is also enough.
    {
        std::unordered_set<CourseId> done;
        done.insert(CourseId{"CS301"});
        done.insert(CourseId{"Math302"});
        REQUIRE(g.isSatisfied(target, done));
    }

    // Math301 alone (no CS301) is NOT enough — the AND part is missing.
    {
        std::unordered_set<CourseId> done;
        done.insert(CourseId{"Math301"});
        REQUIRE_FALSE(g.isSatisfied(target, done));
    }

    // CS301 alone (no member of the OR group) is NOT enough.
    {
        std::unordered_set<CourseId> done;
        done.insert(CourseId{"CS301"});
        REQUIRE_FALSE(g.isSatisfied(target, done));
    }
}

TEST_CASE("shortestPath returns the BFS-shortest course chain", "[prereq][bfs]") {
    auto g = buildCanonicalCurriculum();

    // CS101 -> CS102 -> CS301 -> AD401 = 4 nodes / 3 hops (the direct
    // CS102 -> CS301 edge wins over the longer CS101 -> CS102 -> CS201 -> CS301
    // route).
    auto path = g.shortestPath(CourseId{"CS101"}, CourseId{"AD401"});
    REQUIRE(path.size() == 4);
    REQUIRE(path.front() == CourseId{"CS101"});
    REQUIRE(path.back() == CourseId{"AD401"});

    // Math101 -> Math201 -> AI301 = 3 nodes.
    auto path2 = g.shortestPath(CourseId{"Math101"}, CourseId{"AI301"});
    REQUIRE(path2.size() == 3);
    REQUIRE(path2.front() == CourseId{"Math101"});
    REQUIRE(path2.back() == CourseId{"AI301"});

    // Trivial: from == to is a single-node path.
    auto path3 = g.shortestPath(CourseId{"CS101"}, CourseId{"CS101"});
    REQUIRE(path3.size() == 1);
}

TEST_CASE("shortestPath is empty when target is unreachable", "[prereq][bfs]") {
    auto g = buildCanonicalCurriculum();
    // Stats301's prereq subtree (Math201 -> Math101) does not reach CS301.
    REQUIRE(g.shortestPath(CourseId{"CS301"}, CourseId{"Stats301"}).empty());
    // Unknown nodes also yield empty.
    REQUIRE(g.shortestPath(CourseId{"NONE-A"}, CourseId{"CS101"}).empty());
}

TEST_CASE("removePrereq round-trips and re-adding works", "[prereq][mutation]") {
    PrerequisiteGraph g;
    g.addPrereq(CourseId{"CS102"}, CourseId{"CS101"});
    REQUIRE(g.transitiveClosure(CourseId{"CS102"}).size() == 1);

    REQUIRE(g.removePrereq(CourseId{"CS102"}, CourseId{"CS101"}));
    REQUIRE(g.transitiveClosure(CourseId{"CS102"}).empty());

    // Removing a non-edge returns false.
    REQUIRE_FALSE(g.removePrereq(CourseId{"CS102"}, CourseId{"CS101"}));

    // Re-add succeeds.
    g.addPrereq(CourseId{"CS102"}, CourseId{"CS101"});
    REQUIRE(g.transitiveClosure(CourseId{"CS102"}).size() == 1);
}

TEST_CASE("Closure-cache invalidation propagates to descendants", "[prereq][cache][mutation]") {
    PrerequisiteGraph g;
    g.addPrereq(CourseId{"B"}, CourseId{"A"});
    g.addPrereq(CourseId{"C"}, CourseId{"B"});

    // Prime the cache for C: should see {B, A}.
    REQUIRE(g.transitiveClosure(CourseId{"C"}).size() == 2);

    // Add a new prereq deep in B's ancestry — C's cache must invalidate so the
    // next call reflects the new ancestor.
    g.addPrereq(CourseId{"A"}, CourseId{"Z"});
    auto closure = g.transitiveClosure(CourseId{"C"});
    REQUIRE(closure.size() == 3);
    REQUIRE(contains(closure, CourseId{"A"}));
    REQUIRE(contains(closure, CourseId{"B"}));
    REQUIRE(contains(closure, CourseId{"Z"}));
}

TEST_CASE("addCourse alone registers a node with no edges", "[prereq][mutation]") {
    PrerequisiteGraph g;
    g.addCourse(CourseId{"CS101"});
    REQUIRE(g.courseCount() == 1);
    REQUIRE(g.transitiveClosure(CourseId{"CS101"}).empty());
    auto order = g.topologicalOrder();
    REQUIRE(order.size() == 1);
    REQUIRE(order.front() == CourseId{"CS101"});
}
