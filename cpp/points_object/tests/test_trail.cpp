#include "test_helper.hpp"
#include "../src/trail.hpp"

TEST(test_trail_starts_at_start) {
    Trail t(Point{0, 0});
    ASSERT_EQ(t.coords().size(), 2u);
    ASSERT_EQ(t.coords()[0], 0.0);
    ASSERT_EQ(t.history().size(), 1u);
}

TEST(test_step_updates_position) {
    Trail t(Point{0, 0});
    t.step({3, 4});
    ASSERT_EQ(t.coords()[0], 3.0);
    ASSERT_EQ(t.coords()[1], 4.0);
}

TEST(test_step_appends_to_history) {
    Trail t(Point{0, 0});
    t.step({1, 1});
    t.step({2, 2});
    ASSERT_EQ(t.history().size(), 3u);
    ASSERT_TRUE(t.history()[0] == (Point{0, 0}));
    ASSERT_TRUE(t.history()[1] == (Point{1, 1}));
    ASSERT_TRUE(t.history()[2] == (Point{3, 3}));
}

TEST(test_trail_works_with_distance) {
    Trail t(Point{0, 0});
    t.step({3, 4});
    ASSERT_NEAR((Point{0, 0}).distance_to(t), 5.0, 1e-9);
}

TEST(test_trail_is_any_dimension) {
    Trail t(Point{1, 1, 1, 1});
    t.step({1, 1, 1, 1});
    ASSERT_EQ(t.coords().size(), 4u);
    ASSERT_EQ(t.coords()[0], 2.0);
}

void run_trail_tests() {
    std::cout << "test_trail:\n";
    RUN(test_trail_starts_at_start);
    RUN(test_step_updates_position);
    RUN(test_step_appends_to_history);
    RUN(test_trail_works_with_distance);
    RUN(test_trail_is_any_dimension);
}
