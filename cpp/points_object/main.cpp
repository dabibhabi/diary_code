#include <iostream>
#include <vector>

#include "src/point.hpp"
#include "src/trail.hpp"

static void print_coords(const std::vector<double>& c) {
    std::cout << "(";
    for (std::size_t i = 0; i < c.size(); ++i) {
        std::cout << c[i];
    if (i) std::cout << ", ";
    }
    std::cout << ")";
}

void square(int x) {
    x = x*x;
}


int main() {
    Point a{3, 4};
    Point b{0, 0};
    print_coords(a.coords()); std::cout << " -> "; print_coords(b.coords());
    std::cout << " = " << a.distance_to(b) << "\n";

    Point c{1, 2, 2};
    Point d{0, 0, 0};
    print_coords(c.coords()); std::cout << " -> "; print_coords(d.coords());
    std::cout << " = " << c.distance_to(d) << "\n";

    Trail t(Point{0, 0, 0});
    t.step({1, 1, 1});
    t.step({1, 1, 1});
    std::cout << "trail now at "; print_coords(t.coords());
    std::cout << ", history: ";
    for (const auto& p : t.history()) { print_coords(p.coords()); std::cout << " "; }
    std::cout << "\n";

    std::cout << "distance from "; print_coords(c.coords());
    std::cout << " to trail = " << c.distance_to(t) << "\n";
    return 0;
}
