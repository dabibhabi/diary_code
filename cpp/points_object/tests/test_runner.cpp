#include <iostream>

#include "test_helper.hpp"

extern void run_point_tests();
extern void run_locatable_tests();
extern void run_trail_tests();

int main() {
    run_point_tests();
    run_locatable_tests();
    run_trail_tests();
    std::cout << "\n" << tests_passed << " passed, " << tests_failed << " failed\n";
    return tests_failed == 0 ? 0 : 1;
}
