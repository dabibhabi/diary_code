#include "test_helper.hpp"
#include "../src/point.hpp"

TEST(test_distance_2d) {
    ASSERT_NEAR((Point{3, 4}).distance_to(Point{0, 0}), 5.0, 1e-9);
}

TEST(test_distance_3d) {
    ASSERT_NEAR((Point{1, 2, 2}).distance_to(Point{0, 0, 0}), 3.0, 1e-9);
}

TEST(test_distance_is_symmetric) {
    Point a{1, 2, 3};
    Point b{4, 5, 6};
    ASSERT_NEAR(a.distance_to(b), b.distance_to(a), 1e-9);
}

TEST(test_distance_to_self_is_zero) {
    ASSERT_NEAR((Point{7, 8, 9}).distance_to(Point{7, 8, 9}), 0.0, 1e-9);
}

TEST(test_mismatched_dimensions_raise) {
    Point a{1, 2};
    Point b{1, 2, 3};
    ASSERT_THROWS(a.distance_to(b));
}

void run_locatable_tests() {
    std::cout << "test_locatable:\n";
    RUN(test_distance_2d);
    RUN(test_distance_3d);
    RUN(test_distance_is_symmetric);
    RUN(test_distance_to_self_is_zero);
    RUN(test_mismatched_dimensions_raise);
}
