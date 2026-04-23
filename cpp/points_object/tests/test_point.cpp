#include "test_helper.hpp"
#include "../src/point.hpp"

TEST(test_point_stores_coords) {
    Point p{3, 4};
    ASSERT_EQ(p.coords().size(), 2u);
    ASSERT_EQ(p.coords()[0], 3.0);
    ASSERT_EQ(p.coords()[1], 4.0);
}

TEST(test_point_is_any_dimension) {
    ASSERT_EQ((Point{1}).dimension(), 1u);
    ASSERT_EQ((Point{1, 2}).dimension(), 2u);
    ASSERT_EQ((Point{1, 2, 3, 4, 5}).dimension(), 5u);
}

TEST(test_point_equality) {
    ASSERT_TRUE((Point{1, 2}) == (Point{1, 2}));
    ASSERT_TRUE((Point{1, 2}) != (Point{2, 1}));
}

void run_point_tests() {
    std::cout << "test_point:\n";
    RUN(test_point_stores_coords);
    RUN(test_point_is_any_dimension);
    RUN(test_point_equality);
}
