#include "Vec3/Vec3.hpp"

void square(double &x) {
  x = x*x;
}

int main() {
    Vec3 v1(1.0f, 2.0f, 3.0f);
    Vec3 v2(4.0f, 5.0f, 6.0f);
    Vec3 v3 = v1 + v2;
    square(v3.x);
    square(v3.y);
    square(v3.z);
}

