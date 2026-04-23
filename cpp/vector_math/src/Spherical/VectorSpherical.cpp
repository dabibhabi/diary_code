#include "Vec3/Spherical/VectorSpherical.hpp"

/**
 * @brief Default constructor. Initializes all components to zero.
 */
VectorSpherical::VectorSpherical() : r(0), theta(0), phi(0) {}

/**
 * @brief Constructs a spherical vector with specified components.
 * @param r     Radius (distance from origin).
 * @param theta Polar angle from the +z axis in radians.
 * @param phi   Azimuthal angle from the +x axis in radians.
 */
VectorSpherical::VectorSpherical(double r, double theta, double phi) : r(r), theta(theta), phi(phi) {}
