#include "Vec3/Frenet/VectorFrenet.hpp"

/**
 * @brief Default constructor. Initializes all components to zero.
 */
VectorFrenet::VectorFrenet() : s(0), d(0), theta(0) {}

/**
 * @brief Constructs a Frenet vector with specified components.
 * @param s     Arc length along the path centerline.
 * @param d     Lateral offset from the centerline (positive = left).
 * @param theta Heading relative to the path tangent in radians.
 */
VectorFrenet::VectorFrenet(double s, double d, double theta) : s(s), d(d), theta(theta) {}

